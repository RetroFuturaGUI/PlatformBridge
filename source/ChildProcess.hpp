#pragma once
#include <array>
#include <chrono>
#include <concepts>
#include <cstdint>
#include <filesystem>
#include <span>
#include <string_view>

namespace PlatformBridge
{
    /// @brief A program started with pipes to its standard I/O and tarted with every restriction (Sandboxed). When it
    ///        crashes or is taken over by a malicious file, the parent process remains unaffected. If started with no
    ///        restrictions (ChildProcessFlags::None) it runs with its own rights (administrator rights included if 
    ///        the process has them) has them. The restrictions are Windows only. On Linux it's kept apart only
    class ChildProcess
    {
    public:
        /// @brief What the program may not do. Each restriction is Windows only
        enum class ChildProcessFlags : std::uint32_t
        {
            None = 0,               // Parent process's rights, administrator rights included when it has them
            LowIntegrity = 1,       // low integrity, without administrator rights or privileges. It reads files but can't change the user's ones
            NoChildProcesses = 1 << 1, // Forbidden to start its own programs
            MemoryLimit = 1 << 2,   // its memory is capped at _memoryLimit
            EndsWithThisProcess = 1 << 3, // it ends when the parent process ends
            Sandboxed = LowIntegrity | NoChildProcesses | MemoryLimit | EndsWithThisProcess
        };

        friend constexpr ChildProcessFlags operator |(const ChildProcessFlags left, const ChildProcessFlags right)
        {
            return static_cast<ChildProcessFlags>(static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right));
        }

        friend constexpr ChildProcessFlags operator &(const ChildProcessFlags left, const ChildProcessFlags right)
        {
            return static_cast<ChildProcessFlags>(static_cast<std::uint32_t>(left) & static_cast<std::uint32_t>(right));
        }

        /// @brief Returns whether every bit of flag (param 2) is set in flags (param 1)
        static constexpr bool HasFlag(const ChildProcessFlags flags, const ChildProcessFlags flag)
        {
            return static_cast<std::uint32_t>(flag) == (static_cast<std::uint32_t>(flags) & static_cast<std::uint32_t>(flag));
        }

        ChildProcess() = default;
        ChildProcess(const ChildProcess&) = delete;
        ChildProcess(ChildProcess&&) = delete;
        ChildProcess& operator=(const ChildProcess&) = delete;
        ChildProcess& operator=(ChildProcess&&) = delete;

        /// @brief Stops the program, unless it was started without EndsWithThisProcess, which leaves it running on its own
        ~ChildProcess();

        /// @brief Starts the program with the given arguments (UTF-8), without a window or error output. Stops any previously
        ///        started program. Arguments are passed directly, without a shell, but other processes may see them in the
        ///        command line. Send secrets with Write instead
        template<typename... Args> requires (std::convertible_to<const Args&, std::string_view> && ...)
        bool Start(const std::filesystem::path& program, const ChildProcessFlags flags, const Args&... arguments)
        {
            const std::array<std::string_view, sizeof...(Args)> list { std::string_view(arguments)... };
            return start(program, flags, list);
        }

        /// @brief Ends the program at once. Additional calls are Idempotent
        void Stop();

        bool IsRunning() const;

        /// @brief Writes all of data to the program's standard input. False when it has ended
        bool Write(std::span<const uint8_t> data);

        /// @brief Reads exactly data.size() bytes from the program's standard output. False when it ended first, or when they
        ///        don't arrive within timeout (the program is stopped, since it hangs)
        bool Read(std::span<std::uint8_t> data, const std::chrono::milliseconds timeout);

    private:
        bool start(const std::filesystem::path& program, const ChildProcessFlags flags, std::span<const std::string_view> arguments);

        /// @brief Closes every handle without waiting for anything
        void closeHandles();

        static constexpr std::intptr_t _noHandle { -1 };
        static constexpr size_t _memoryLimit { 1024uz * 1024 * 1024 }; //enough to decode, likely too little for a malicious file to take the machine's memory

        std::intptr_t
            _process { _noHandle },  //Windows: process handle; Linux: pid
            _input { _noHandle },    //the writing end of the program's standard input
            _output { _noHandle },   //the reading end of its standard output
            _job { _noHandle },      //Windows: the job holding the restrictions that need one
            _readEvent { _noHandle }; //Windows: signals the overlapped reads
        ChildProcessFlags _flags { ChildProcessFlags::None }; //what the running program was started with
    };
}