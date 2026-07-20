#include "PlatformBridge.hpp"
#include <chrono>
#include <print>
#include <format>

int main()
{
    const auto startTime = std::chrono::steady_clock::now();
    PlatformBridge::RefreshPlatformBridge();

    std::println("Installed Fonts:");
    for(const auto& font : PlatformBridge::Fonts::GetFontProperties())
    {
        /*std::println("Name: {}\n    Path: {},\n    Style: {},\n    Weight: {},\n    Slant: {}",
            font._Name, font._Path, font._Style, static_cast<int32_t>(font._Weight), static_cast<int32_t>(font._Slant));

        std::println("    Unicode Ranges:");
        for(const auto& range : font._UnicodeRanges)
        {
            std::println("        U+{:06X} - U+{:06X}", range.first, range.second);
        }

        std::println();*/

        if(font._Name.find("Arial") != std::string::npos /*|| font._Name.find("Comic") != std::string::npos || font._Name.find("Myanmar") != std::string::npos*/)
        {// "\033[38;2;255;127;0mThis is orange text\033[0m\n";
            std::println("\033[127;40;255;127;0mName:\033[0m {}\n    Path: {},\n    Style: {},\n    Weight: {},\n    Slant: {}",
            font._Name, font._Path, font._Style, static_cast<int32_t>(font._Weight), static_cast<int32_t>(font._Slant));

            std::println("    Unicode Ranges:");
            for(const auto& range : font._UnicodeRanges)
            {
                std::println("        U+{:06X} - U+{:06X}", range.first, range.second);
            }

            std::println();
        }
    }

    const auto endTime = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::println("Benchmark: {} ms", elapsedMs);

    return 0;
}