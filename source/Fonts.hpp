#pragma once
#include <string>
#include <filesystem>
#include <utility>
#include <print>
#include <vector>

namespace PlatformBridge
{
    class Fonts
    {
    public:
        enum class Slant : int32_t
        {
            Roman = 0,
            Italic = 1,
            Oblique = 2
        };

        enum class Weight : int32_t
        {
            Thin = 100,
            ExtraLight = 200,
            Light = 300,
            Normal = 400,
            Regular = Normal,
            Medium = 500,
            SemiBold = 600,
            Bold = 700,
            ExtraBold = 800,
            Black = 900
        };
        
        struct FontProperty
        {
            std::string 
                _Name {}, //aka Family
                _Path {},
                _Style {};
            Weight _Weight { Weight::Normal };
            Slant _Slant { Slant::Roman };
            std::vector<std::pair<uint32_t, uint32_t>> _UnicodeRanges {};
        };

        /// @brief Refreshes all data of installed fonts.
        static void Refresh()
        {
            setFontsInformation();
        }

        /// @brief Refreshes all data of installed fonts.
        /// @return Returns an std::vector<FontProperty> containing information of all installed fonts 
        static const std::vector<FontProperty>& GetFontProperties()
        {
            if(_fontProperties.empty() || _fontProperties.size() == 0)
                Refresh();
                
            return _fontProperties;
        }

    private:
        Fonts() = default;
        ~Fonts() = default;
        Fonts(const Fonts&) = delete;
        Fonts(Fonts&&) = delete;
        void operator=(const Fonts&) = delete;
        void operator=(Fonts&&) = delete;
        static Fonts& GetInstance()
        {
            static Fonts Instance;
            return Instance;
        }

        static inline std::vector<FontProperty> _fontProperties {}; 

        static void setFontsInformation();
        static Slant getSlantValue(const int32_t slant);
        static Weight getCssWeightValue(const int32_t weight);
        static std::vector<std::pair<uint32_t, uint32_t>> getUnicodeRanges(void* charset);
    };
}