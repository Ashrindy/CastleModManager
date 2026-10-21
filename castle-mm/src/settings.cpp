#include "settings.h"
#include <INIReader.h>
#include <fstream>

Settings* Settings::instance{ nullptr };

#define SERIALIZE_VALUE(NAME, VALUE) settings << std::string{ NAME } << "=" << VALUE << '\n'

void Settings::serialize(const std::filesystem::path& filepath) {
    std::ofstream settings{ filepath };

    settings << "[Main]\n";
    SERIALIZE_VALUE("ModsFolderPath", modsFolderPath);
    SERIALIZE_VALUE("GameFolderPath", gameFolderPath);
    SERIALIZE_VALUE("LanguageName", languageName);
}

void Settings::deserialize(const std::filesystem::path& filepath) {
    if (std::filesystem::exists(filepath)) {
        INIReader settings{ filepath.string() };
        
        modsFolderPath = settings.GetString("Main", "ModsFolderPath", "mods");
        gameFolderPath = settings.GetString("Main", "GameFolderPath", "C:\\CastleStrike");
        languageName = settings.GetString("Main", "LanguageName", "en-US");
    }
}
