#include "settings.h"
#include <INIReader.h>
#include <fstream>

Settings* Settings::instance{ nullptr };

#define SERIALIZE_STRING_VALUE(NAME, VALUE) settings << std::string{ NAME } << "=" << VALUE << '\n'
#define SERIALIZE_VALUE(NAME, VALUE) settings << std::string{ NAME } << "=" << std::to_string(VALUE) << '\n'

void Settings::serialize(const std::filesystem::path& filepath) {
#ifdef ENABLE_FLATPAK
    std::ofstream settings{ std::filesystem::path{ std::getenv("HOME") } / ".var/app/io.github.ashrindy.castlemodmanager/settings.ini" };
#else
    std::ofstream settings{ filepath };
#endif

    settings << "[Main]\n";
    SERIALIZE_STRING_VALUE("ModsFolderPath", modsFolderPath);
    SERIALIZE_STRING_VALUE("GameFolderPath", gameFolderPath);
    SERIALIZE_STRING_VALUE("LanguageName", languageName);
    SERIALIZE_VALUE("AutoFetchModLoader", autoFetch);
}

void Settings::deserialize(const std::filesystem::path& filepath) {
#ifdef ENABLE_FLATPAK
    INIReader settings{ std::filesystem::path{ std::getenv("HOME") } / ".var/app/io.github.ashrindy.castlemodmanager/settings.ini" };
#else
    INIReader settings{ filepath.string() };
#endif
        
    modsFolderPath = settings.GetString("Main", "ModsFolderPath", "mods");
    gameFolderPath = settings.GetString("Main", "GameFolderPath", "C:\\CastleStrike");
    languageName = settings.GetString("Main", "LanguageName", "en-US");
    autoFetch = settings.GetBoolean("Main", "AutoFetchModLoader", false);
}
