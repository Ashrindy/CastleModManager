#pragma once
#include <string>

class Settings {
public:
    std::string modsFolderPath{};
    std::string gameFolderPath{};
    std::string languageName{};
    bool autoFetch{ true };

    inline void init() {
        instance = this;
    }
    
    void serialize(const std::filesystem::path& filepath = "settings.ini");
    void deserialize(const std::filesystem::path& filepath = "settings.ini");

    static Settings* instance;
};
