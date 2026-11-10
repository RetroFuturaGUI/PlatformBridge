#pragma once
#include <string>
#include <filesystem>
#include <map>
#include <mutex>
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
            uint32_t _FaceIndex { 0 }; //which face of a collection file (.ttc) this is; 0 for a file with one face
        };

        /// @brief Refreshes all data of installed fonts
        static void Refresh()
        {
            setFontsInformation();
        }

        /// @brief Refreshes all data of installed fonts
        /// @return Returns an std::vector<FontProperty> containing information of all installed fonts 
        static const std::vector<FontProperty>& GetFontProperties()
        {
            if(_fontProperties.empty() || _fontProperties.size() == 0)
                Refresh();
                
            return _fontProperties;
        }

        /// @brief Returns the code-points the font covers, as ranges of first and last code-point. Read from its file the first
        ///        time they are asked for, since few programs need them and reading them for every installed font is slow. Kept
        ///        until the next Refresh, which the returned reference doesn't outlive. Empty when the font has no Unicode character map
        static const std::vector<std::pair<uint32_t, uint32_t>>& GetUnicodeRanges(const FontProperty& font);

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
        static inline std::map<std::pair<std::string, uint32_t>, std::vector<std::pair<uint32_t, uint32_t>>> _unicodeRanges {}; //by path and face index, filled as they are asked for
        static inline std::mutex _unicodeRangesMutex {};

        static void setFontsInformation();
        static Slant getSlantValue(const int32_t slant);
        static Weight getCssWeightValue(const int32_t weight);
        static std::vector<std::pair<uint32_t, uint32_t>> getUnicodeRanges(void* charset);
    };
}