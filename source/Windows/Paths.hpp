#include <filesystem>
#include <Windows.h>

namespace PlatformBridge
{
    class Paths
    {
    public:
        static std::string GetExecutablePath();
        static const char* GetExecutablePathCStr();

    private:
        Paths();
        ~Paths() = default;
        Paths(const Paths&) = delete;
        Paths(Paths&&) = delete;
        void operator=(const Paths&) = delete;
        void operator=(Paths&&) = delete;
        static Paths& GetInstance()
        {
            static Paths Instance;
            return Instance;
        }


        static void setExecutablePath();

        static inline std::filesystem::path _exePath;

    };
}