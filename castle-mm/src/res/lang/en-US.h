#pragma once
#include <utility>

static std::pair<const char*, const char*> en_US[] {
    // General
    { "general_add", "Add" },
    { "general_remove", "Remove" },
    { "general_name", "Name" },
    { "general_author", "Author" },
    { "general_version", "Version" },
    { "general_main", "Main" },
    { "general_details", "Details" },
    // Tab names
    { "mods-tab-name", "Mods" },
    { "settings-tab-name", "Settings" },
    // Main buttons
    { "save-button-name", "Save" },
    { "save-and-play-button-name", "Save and Play" },
    // Mods tab
    { "mods-tab_deletemod", "Delete mod" },
    { "mods-tab_deletemod-tooltip", "Deletes all of the mod's contents." },
    { "mods-tab_revealmod", "Open mod's directory" },
    { "mods-tab_editmod", "Edit mod" },
    { "mods-tab_refresh-list", "Refresh List" },
    { "mods-tab_create-mod", "Create Mod" },
    // Mod Details
    { "mod-details_name", "Mod Details" },
    { "mod-details_dllfiles", "DLL Files" },
    // Settings tab
    { "settings-tab_modmanager-sectionname", "Mod Manager" },
    { "settings-tab_modmanager-mods_folder", "Mods Folder" },
    { "settings-tab_modmanager-game_folder", "Game Folder" },
    { "settings-tab_modmanager-language", "Language" },
    
    { "settings-tab_modloader-sectionname", "Mod Loader" },
    { "settings-tab_modloader-fetch", "Fetch" },
    { "settings-tab_modloader-download", "Download" },
    { "settings-tab_modloader-uninstall", "Uninstall" },
    { "settings-tab_modloader-status", "Status" },
    { "settings-tab_modloader-downloadedver", "Downloaded version" },
    { "settings-tab_modloader-downloadedver_refresh", "Refresh" },
    { "settings-tab_modloader-fetch_fail", "Failed to fetch." },
    { "settings-tab_modloader-fetch_exists", "The latest version is already downloaded." },
    { "settings-tab_modloader-fetch_noloader", "No mod loader is downloaded." },
    { "settings-tab_modloader-fetch_newver", "There's a newer version available." },
    { "settings-tab_modloader-download_success", "Successfully downloaded latest version." },
    { "settings-tab_modloader-download_fail", "Failed to download." },
};
