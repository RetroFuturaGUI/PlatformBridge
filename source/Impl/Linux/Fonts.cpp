#include "Fonts.hpp"
#include "LinuxHeaders.hpp"

void PlatformBridge::Fonts::Refresh()
{
    setFontsInformation();
}

const std::vector<std::pair<std::string, std::string>>& PlatformBridge::Fonts::GetFontsInformation()
{
    if(_fontInformation.empty() || _fontInformation.size() == 0)
        Refresh();
        
    return _fontInformation;
}

void PlatformBridge::Fonts::setFontsInformation()
{
    _fontInformation.clear();

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
        size_t separatorPos = line.find_first_of(": ");

        if(separatorPos == -1uz)
            continue;

        _fontInformation.emplace_back(std::string(line.begin() + separatorPos + 2, line.end() - 1), std::string(line.begin(), line.begin() + separatorPos));
    }

    pclose(pipe);
}