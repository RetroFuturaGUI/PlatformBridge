#include "Fonts.hpp"
#include <fontconfig/fontconfig.h>

void PlatformBridge::Fonts::Refresh()
{
    setFontsInformation();
}

const std::vector<PlatformBridge::Fonts::FontProperty>& PlatformBridge::Fonts::GetFontProperties()
{
    if(_fontProperties.empty() || _fontProperties.size() == 0)
        Refresh();
        
    return _fontProperties;
}

void PlatformBridge::Fonts::setFontsInformation()
{
    _fontProperties.clear();

    FcInit();
    FcConfig* fcConfig { FcInitLoadConfigAndFonts() };
    FcPattern* fcPattern { FcPatternCreate() };
    FcObjectSet* objectSet { FcObjectSetBuild(FC_FILE, 
        FC_FAMILY, 
        FC_SLANT,
        FC_WEIGHT,
        FC_STYLE,
        FC_CHARSET,
        //FC_FULLNAME,
        //FC_FONT_VARIATIONS,
        //FC_FONT_WRAPPER,
        //FC_FAMILYLANG,
        //FC_LANG,
        nullptr
    )};
    
    FcFontSet* fcFontList { FcFontList(fcConfig, fcPattern, objectSet) };
    //FcLangSet* langs = FcLangSetCreate();

    for (int i = 0; fcFontList && i < fcFontList->nfont; ++i)
    {
        FcPattern* font { fcFontList->fonts[i] };
        FcChar8* file { nullptr }, 
            *style { nullptr },
            *family { nullptr };
            //lang { nullptr },
            //*fullname { nullptr },
            //*wrapper { nullptr },
            //*variations { nullptr },
            //*postscriptname;
        FcCharSet* fcCharset { nullptr };
        int32_t
            slant { 0 }, 
            weight { 0 };

        if(FcPatternGetString(font, FC_FILE, 0, &file) != FcResultMatch)
            continue;

        if(FcPatternGetString(font, FC_FAMILY, 0, &family) != FcResultMatch)
            continue;

        if(FcPatternGetInteger(font, FC_SLANT, 0, &slant) != FcResultMatch)
            continue;

        if(FcPatternGetInteger(font, FC_WEIGHT, 0, &weight) != FcResultMatch)
            continue;

        if(FcPatternGetString(font, FC_STYLE, 0, &style) != FcResultMatch)
            continue;

        /*
        if(FcPatternGetString(font, FC_POSTSCRIPT_NAME, 0, &postscriptname) != FcResultMatch)
            continue;

        if(FcPatternGetString(font, FC_LANG, 0, &langs) != FcResultMatch)
            continue;

        if(FcPatternGetString(font, FC_FULLNAME, 0, &fullname) != FcResultMatch)
            continue;

        if(FcPatternGetString(font, FC_FONT_VARIATIONS, 0, &variations) != FcResultMatch)
            continue;

        if(FcPatternGetString(font, FC_FONT_WRAPPER, 0, &wrapper) != FcResultMatch)
            continue;

        if(FcPatternGetString(font, FC_FAMILYLANG, 0, &lang) != FcResultMatch)
            continue;
        */ 

        if (std::string((char*)file).find(".gz") != std::string::npos)
            continue;

        if (FcPatternGetCharSet(font, FC_CHARSET, 0, &fcCharset) != FcResultMatch)
            continue;

        _fontProperties.emplace_back(FontProperty
        {
            ._Name = std::string((char*)family),
            ._Path = std::string((char*)file),
            ._Style = std::string((char*)style),
            ._Weight = getCssWeightValue(weight),
            ._Slant = getSlantValue(slant),
            ._UnicodeRanges = getUnicodeRanges(fcCharset)
        });
            
    }
    

    if (fcFontList) 
        FcFontSetDestroy(fcFontList);

    FcFini();
}

PlatformBridge::Fonts::Slant PlatformBridge::Fonts::getSlantValue(const int32_t slant)
{
    switch(slant)
    {
        case FC_SLANT_ITALIC: 
            return Slant::Italic;
        case FC_SLANT_OBLIQUE:
            return Slant::Oblique;
        default: 
            return Slant::Roman;
    }
}

PlatformBridge::Fonts::Weight PlatformBridge::Fonts::getCssWeightValue(const int32_t weight)
{
    switch(weight)
    {
        case FC_WEIGHT_THIN:
            return Weight::Thin;
        case FC_WEIGHT_EXTRALIGHT: //aka FC_WEIGHT_ULTRALIGHT
            return Weight::ExtraLight;
        case FC_WEIGHT_LIGHT: case FC_WEIGHT_DEMILIGHT: //aka FC_WEIGHT_SEMILIGHT
            return Weight::Light;
        case FC_WEIGHT_MEDIUM:
            return Weight::Medium;
        case FC_WEIGHT_SEMIBOLD: //aka FC_WEIGHT_DEMIBOLD
            return Weight::SemiBold;
        case FC_WEIGHT_BOLD:
            return Weight::Bold;
        case FC_WEIGHT_EXTRABOLD: //aka FC_WEIGHT_ULTRABOLD
            return Weight::ExtraBold;
        case FC_WEIGHT_BLACK: // aka FC_WEIGHT_HEAVY, FC_WEIGHT_EXTRABLACK, FC_WEIGHT_ULTRABLACK
            return Weight::Black;
        default: // FC_WEIGHT_BOOK, FC_WEIGHT_REGULAR, FC_WEIGHT_NORMAL
            return Weight::Normal;
    }
}

std::vector<std::pair<uint32_t, uint32_t>> PlatformBridge::Fonts::getUnicodeRanges(void* charset)
{
    FcCharSet* fcCharset = reinterpret_cast<FcCharSet*>(charset);
    uint32_t codepointlast { 0 };
    std::vector<std::pair<uint32_t, uint32_t>> ranges {};
    FcChar32 
        fcNextChar { 0 },
        fcMap[FC_CHARSET_MAP_SIZE],
        fcPage { FcCharSetFirstPage(fcCharset, fcMap, &fcNextChar) };

    while(fcPage != FC_CHARSET_DONE)
    {
        for (size_t w = FC_CHARSET_MAP_SIZE; w-- > 0;) 
        {
            if (!fcMap[w])
                continue;

            int32_t msb = 31 - __builtin_clz(fcMap[w]);
            codepointlast = fcPage + w * 32 + msb;
            break; // highest set bit in this page found
        }

        ranges.emplace_back(fcPage, codepointlast);
        fcPage = FcCharSetNextPage(fcCharset, fcMap, &fcNextChar);
    }

    return ranges;
}

/*old implementation
void PlatformBridge::Fonts::setFontsInformation()
{
    _fontProperties.clear();

    FILE* pipe = popen("fc-list", "r");

    if(!pipe)
    {
        std::println("Error listing fonts.");
        return;
    }

    char buffer[512];

    while(fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        std::string line(buffer);
        size_t tempPos = line.find('\n');

        if(tempPos == std::string::npos)
            continue;

        tempPos = line.find_first_of(": ");
        if(tempPos == std::string::npos)
            continue;

        if(line.find(".ttf") == std::string::npos 
            && line.find(".ttc") == std::string::npos
            && line.find(".otf") == std::string::npos)
            continue;

        _fontProperties.emplace_back(FontProperty(std::string(line.begin() + tempPos + 2, line.end() - 1), std::string(line.begin(), line.begin() + tempPos)));
    }

    pclose(pipe);
}
*/