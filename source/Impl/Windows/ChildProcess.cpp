#include "ChildProcess.hpp"
#include <Windows.h>
#include <sddl.h>
#include <atomic>
#include <format>
#include <string>
#include <vector>

//UTF-8 to the UTF-16 Windows takes
static std::wstring toWide(std::string_view text)
{
    if(text.empty())
        return {};

    // move RetroFuturaGUI's conversion here or build a conversion only version of MorphText
    std::wstring wide(static_cast<std::size_t>(MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0)), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), wide.data(), static_cast<int>(wide.size()));
    return wide;
}

//One argument appended so the program's CommandLineToArgvW gets it back as it was. Quoted when it holds blanks or quotes or
//is empty, inner quotes escaped, and the backslashes before a quote. The closing one included doubled
static void appendArgument(std::wstring& commandLine, std::wstring_view argument)
{
    commandLine.push_back(L' ');

    if(!argument.empty() && std::wstring_view::npos == argument.find_first_of(L" \t\n\v\""))
    {
        commandLine.append(argument);
        return;
    }

    commandLine.push_back(L'"');
    std::size_t backslashCount { 0 };

    for(const wchar_t character : argument)
    {
        if(L'\\' == character)
        {
            ++backslashCount;
            continue;
        }

        // Backslashes count as such only before a quote
        commandLine.append(L'"' == character ? backslashCount * 2 + 1 : backslashCount, L'\\');
        backslashCount = 0;
        commandLine.push_back(character);
    }

    commandLine.append(backslashCount * 2, L'\\');
    commandLine.push_back(L'"');
}

PlatformBridge::ChildProcess::~ChildProcess()
{
    // Started to outlive this process, it outlives this object too
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

    // The program's standard output comes back through a named pipe opened for overlapped reads, so a read can wait with a
    // timeout; anonymous pipes can't. Its name only has to be unique within this process's life
    static std::atomic<std::uint32_t> pipeCount { 0 };
    const std::wstring pipeName { std::format(L"\\\\.\\pipe\\RetroFuturaGUI-{}-{}", GetCurrentProcessId(), ++pipeCount) };
    constexpr DWORD pipeBufferSize { 1024 * 1024 };
    SECURITY_ATTRIBUTES inheritable { .nLength = sizeof(SECURITY_ATTRIBUTES), .lpSecurityDescriptor = nullptr, .bInheritHandle = TRUE };

    HANDLE output { CreateNamedPipeW(pipeName.c_str(), PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE,
        PIPE_TYPE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS, 1, pipeBufferSize, pipeBufferSize, 0, nullptr) };

    if(INVALID_HANDLE_VALUE == output)
        return false;

    _output = reinterpret_cast<std::intptr_t>(output);
    HANDLE childOutput { CreateFileW(pipeName.c_str(), GENERIC_WRITE, 0, &inheritable, OPEN_EXISTING, 0, nullptr) };
    HANDLE childInput { nullptr }, input { nullptr };
    HANDLE nothing { CreateFileW(L"NUL", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &inheritable, OPEN_EXISTING, 0, nullptr) };

    const auto closeChildSide { [&]()
        {
            if(INVALID_HANDLE_VALUE != childOutput)
                CloseHandle(childOutput);

            if(childInput)
                CloseHandle(childInput);

            if(INVALID_HANDLE_VALUE != nothing)
                CloseHandle(nothing);
        } };

    if(INVALID_HANDLE_VALUE == childOutput || INVALID_HANDLE_VALUE == nothing || !CreatePipe(&childInput, &input, &inheritable, 0))
    {
        closeChildSide();
        closeHandles();
        return false;
    }

    _input = reinterpret_cast<std::intptr_t>(input);
    SetHandleInformation(input, HANDLE_FLAG_INHERIT, 0); //only the program's ends are inherited

    // The program inherits its three handles and nothing else this process holds
    HANDLE inherited[] { childInput, childOutput, nothing };
    SIZE_T attributeSize { 0 };
    InitializeProcThreadAttributeList(nullptr, 1, 0, &attributeSize);
    std::vector<std::uint8_t> attributeStorage(attributeSize);
    LPPROC_THREAD_ATTRIBUTE_LIST attributes { reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(attributeStorage.data()) };

    if(!InitializeProcThreadAttributeList(attributes, 1, 0, &attributeSize) || !UpdateProcThreadAttribute(attributes, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, inherited, sizeof(inherited), nullptr, nullptr))
    {
        closeChildSide();
        closeHandles();
        return false;
    }

    STARTUPINFOEXW startup {};
    startup.StartupInfo.cb = sizeof(STARTUPINFOEXW);
    startup.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
    startup.StartupInfo.hStdInput = childInput;
    startup.StartupInfo.hStdOutput = childOutput;
    startup.StartupInfo.hStdError = nothing;
    startup.lpAttributeList = attributes;

    // The program's own path quoted as it is. CommandLineToArgvW reads the first one differently, then the arguments
    std::wstring arguedLine { L"\"" + program.wstring() + L"\"" };

    for(const std::string_view argument : arguments)
        appendArgument(arguedLine, toWide(argument));

    std::wstring commandLine { arguedLine }; //CreateProcess may write into it
    PROCESS_INFORMATION started {};
    constexpr DWORD creationFlags { CREATE_NO_WINDOW | CREATE_SUSPENDED | EXTENDED_STARTUPINFO_PRESENT | CREATE_UNICODE_ENVIRONMENT };
    bool isStarted { false };

    // Low integrity from a restricted copy of this process's own token. Administrators only ever denies, every privilege but the
    // one to traverse folders goes, and the level drops to low, so an elevated parent's child is as limited as a standard one's
    if(HasFlag(flags, ChildProcessFlags::LowIntegrity))
    {
        HANDLE ownToken { nullptr }, restrictedToken { nullptr };
        PSID lowSid { nullptr };
        BYTE administratorsSid[SECURITY_MAX_SID_SIZE] {};
        DWORD administratorsSidSize { sizeof(administratorsSid) };

        if(OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE | TOKEN_QUERY | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_DEFAULT, &ownToken)
            && CreateWellKnownSid(WinBuiltinAdministratorsSid, nullptr, administratorsSid, &administratorsSidSize))
        {
            SID_AND_ATTRIBUTES denied { .Sid = administratorsSid, .Attributes = 0 };

            if(CreateRestrictedToken(ownToken, DISABLE_MAX_PRIVILEGE, 1, &denied, 0, nullptr, 0, nullptr, &restrictedToken)
                && ConvertStringSidToSidW(L"S-1-16-4096", &lowSid)) //the low mandatory level
            {
                TOKEN_MANDATORY_LABEL label { .Label = { .Sid = lowSid, .Attributes = SE_GROUP_INTEGRITY } };

                if(SetTokenInformation(restrictedToken, TokenIntegrityLevel, &label, sizeof(TOKEN_MANDATORY_LABEL) + GetLengthSid(lowSid)))
                    isStarted = CreateProcessAsUserW(restrictedToken, nullptr, commandLine.data(), nullptr, nullptr, TRUE, creationFlags, nullptr, nullptr, &startup.StartupInfo, &started);
            }
        }

        if(lowSid)
            LocalFree(lowSid);

        if(restrictedToken)
            CloseHandle(restrictedToken);

        if(ownToken)
            CloseHandle(ownToken);
    }

    // Without LowIntegrity, or where Windows refused it. This process's own token, administrator rights included when it has them
    if(!isStarted)
    {
        commandLine = arguedLine;
        isStarted = CreateProcessW(nullptr, commandLine.data(), nullptr, nullptr, TRUE, creationFlags, nullptr, nullptr, &startup.StartupInfo, &started);
    }

    DeleteProcThreadAttributeList(attributes);
    closeChildSide(); //the program holds its own copies now

    if(!isStarted)
    {
        closeHandles();
        return false;
    }

    _process = reinterpret_cast<std::intptr_t>(started.hProcess);
    _flags = flags;

    // The other restrictions come from a job it joins before it runs
    DWORD limitFlags { 0 };

    if(HasFlag(flags, ChildProcessFlags::EndsWithThisProcess))
        limitFlags |= JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE; //the job's last handle closes when this process ends

    if(HasFlag(flags, ChildProcessFlags::NoChildProcesses))
        limitFlags |= JOB_OBJECT_LIMIT_ACTIVE_PROCESS;

    if(HasFlag(flags, ChildProcessFlags::MemoryLimit))
        limitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;

    HANDLE job { 0 != limitFlags ? CreateJobObjectW(nullptr, nullptr) : nullptr };

    if(job)
    {
        _job = reinterpret_cast<std::intptr_t>(job);
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits {};
        limits.BasicLimitInformation.LimitFlags = limitFlags;
        limits.BasicLimitInformation.ActiveProcessLimit = 1;
        limits.ProcessMemoryLimit = static_cast<SIZE_T>(_memoryLimit);
        SetInformationJobObject(job, JobObjectExtendedLimitInformation, &limits, sizeof(limits));
        AssignProcessToJobObject(job, started.hProcess);
    }

    ResumeThread(started.hThread);
    CloseHandle(started.hThread);

    HANDLE readEvent { CreateEventW(nullptr, TRUE, FALSE, nullptr) };

    if(!readEvent)
    {
        Stop();
        return false;
    }

    _readEvent = reinterpret_cast<std::intptr_t>(readEvent);
    return true;
}

void PlatformBridge::ChildProcess::Stop()
{
    if(_noHandle != _process)
    {
        TerminateProcess(reinterpret_cast<HANDLE>(_process), 1);
        WaitForSingleObject(reinterpret_cast<HANDLE>(_process), 2000);
    }

    closeHandles();
}

void PlatformBridge::ChildProcess::closeHandles()
{
    for(std::intptr_t* handle : { &_input, &_output, &_readEvent, &_process, &_job })
    {
        if(_noHandle == *handle)
            continue;

        CloseHandle(reinterpret_cast<HANDLE>(*handle));
        *handle = _noHandle;
    }

    _flags = ChildProcessFlags::None;
}

bool PlatformBridge::ChildProcess::IsRunning() const
{
    if(_noHandle == _process)
        return false;

    return WAIT_TIMEOUT == WaitForSingleObject(reinterpret_cast<HANDLE>(_process), 0);
}

bool PlatformBridge::ChildProcess::Write(std::span<const std::uint8_t> data)
{
    if(_noHandle == _input)
        return false;

    while(!data.empty())
    {
        DWORD written { 0 };

        if(!WriteFile(reinterpret_cast<HANDLE>(_input), data.data(), static_cast<DWORD>(std::min<std::size_t>(data.size(), 1u << 30)), &written, nullptr))
            return false; //the program ended and closed its end

        data = data.subspan(written);
    }

    return true;
}

bool PlatformBridge::ChildProcess::Read(std::span<std::uint8_t> data, const std::chrono::milliseconds timeout)
{
    if(_noHandle == _output || _noHandle == _readEvent)
        return false;

    HANDLE output { reinterpret_cast<HANDLE>(_output) };
    const std::chrono::steady_clock::time_point deadline { std::chrono::steady_clock::now() + timeout };

    while(!data.empty())
    {
        OVERLAPPED overlapped {}; //a union inside, so its fields can't all be named
        overlapped.hEvent = reinterpret_cast<HANDLE>(_readEvent);
        ResetEvent(overlapped.hEvent);
        DWORD read { 0 };

        if(!ReadFile(output, data.data(), static_cast<DWORD>(std::min<std::size_t>(data.size(), 1u << 30)), nullptr, &overlapped) && ERROR_IO_PENDING != GetLastError())
            return false; //the program ended

        const std::chrono::milliseconds left { std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now()) };

        if(WAIT_OBJECT_0 != WaitForSingleObject(overlapped.hEvent, static_cast<DWORD>(std::max<std::int64_t>(left.count(), 0))))
        {
            CancelIoEx(output, &overlapped);
            GetOverlappedResult(output, &overlapped, &read, TRUE); //the read may not outlive its buffer
            Stop(); //it hangs
            return false;
        }

        if(!GetOverlappedResult(output, &overlapped, &read, FALSE) || 0 == read)
            return false;

        data = data.subspan(read);
    }

    return true;
}