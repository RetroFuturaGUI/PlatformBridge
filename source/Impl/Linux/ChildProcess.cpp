#include "ChildProcess.hpp"
#include <cerrno>
#include <csignal>
#include <fcntl.h>
#include <poll.h>
#include <spawn.h>
#include <string>
#include <sys/wait.h>
#include <vector>
#include <unistd.h>

extern char** environ;

PlatformBridge::ChildProcess::~ChildProcess()
{
    // Started to outlive this object, it's left running
    if(!HasFlag(_flags, ChildProcessFlags::EndsWithThisProcess))
    {
        closeHandles();
        return;
    }

    Stop();
}

bool PlatformBridge::ChildProcess::start(const std::filesystem::path& program, const ChildProcessFlags flags, std::span<const std::string_view> arguments)
{
    Stop();

    int toChild[2] { -1, -1 }, fromChild[2] { -1, -1 };

    if(0 != pipe2(toChild, O_CLOEXEC))
        return false;

    if(0 != pipe2(fromChild, O_CLOEXEC))
    {
        close(toChild[0]);
        close(toChild[1]);
        return false;
    }

    // The program's ends become its standard input and output; error output goes nowhere
    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);
    posix_spawn_file_actions_adddup2(&actions, toChild[0], STDIN_FILENO);
    posix_spawn_file_actions_adddup2(&actions, fromChild[1], STDOUT_FILENO);
    posix_spawn_file_actions_addopen(&actions, STDERR_FILENO, "/dev/null", O_WRONLY, 0);

    // The arguments go to the program as they are, each its own argv entry; no shell sees them
    const std::string path { program.string() };
    std::vector<std::string> argumentCopies(arguments.begin(), arguments.end());
    std::vector<char*> argumentVector { const_cast<char*>(path.c_str()) };

    for(std::string& argument : argumentCopies)
        argumentVector.push_back(argument.data());

    argumentVector.push_back(nullptr);
    pid_t pid { -1 };
    const int result { posix_spawn(&pid, path.c_str(), &actions, nullptr, argumentVector.data(), environ) };
    posix_spawn_file_actions_destroy(&actions);
    close(toChild[0]);
    close(fromChild[1]);

    if(0 != result)
    {
        close(toChild[1]);
        close(fromChild[0]);
        return false;
    }

    signal(SIGPIPE, SIG_IGN); //a write to a program that ended fails instead of ending this process
    _process = pid;
    _input = toChild[1];
    _output = fromChild[0];
    _flags = flags;
    return true;
}

void PlatformBridge::ChildProcess::Stop()
{
    if(_noHandle != _process)
    {
        kill(static_cast<pid_t>(_process), SIGKILL);
        waitpid(static_cast<pid_t>(_process), nullptr, 0);
    }

    closeHandles();
}

void PlatformBridge::ChildProcess::closeHandles()
{
    for(std::intptr_t* handle : { &_input, &_output })
    {
        if(_noHandle == *handle)
            continue;

        close(static_cast<int>(*handle));
        *handle = _noHandle;
    }

    _process = _noHandle;
    _flags = ChildProcessFlags::None;
}

bool PlatformBridge::ChildProcess::IsRunning() const
{
    if(_noHandle == _process)
        return false;

    return 0 == waitpid(static_cast<pid_t>(_process), nullptr, WNOHANG);
}

bool PlatformBridge::ChildProcess::Write(std::span<const std::uint8_t> data)
{
    if(_noHandle == _input)
        return false;

    while(!data.empty())
    {
        const ssize_t written { write(static_cast<int>(_input), data.data(), data.size()) };

        if(0 > written && EINTR == errno)
            continue;

        if(0 >= written)
            return false;

        data = data.subspan(static_cast<std::size_t>(written));
    }

    return true;
}

bool PlatformBridge::ChildProcess::Read(std::span<std::uint8_t> data, const std::chrono::milliseconds timeout)
{
    if(_noHandle == _output)
        return false;

    const std::chrono::steady_clock::time_point deadline { std::chrono::steady_clock::now() + timeout };

    while(!data.empty())
    {
        const std::chrono::milliseconds left { std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now()) };
        pollfd waitFor { .fd = static_cast<int>(_output), .events = POLLIN, .revents = 0 };
        const int ready { poll(&waitFor, 1, static_cast<int>(std::max<std::int64_t>(left.count(), 0))) };

        if(0 > ready && EINTR == errno)
            continue;

        if(0 == ready)
        {
            Stop(); //it hangs
            return false;
        }

        const ssize_t read { ::read(static_cast<int>(_output), data.data(), data.size()) };

        if(0 > read && EINTR == errno)
            continue;

        if(0 >= read)
            return false; //the program ended

        data = data.subspan(static_cast<std::size_t>(read));
    }

    return true;
}