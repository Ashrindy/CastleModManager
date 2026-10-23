#include "graphics/graphics.h"
#include "../app.h"
#include "../settings.h"
#include "../file-dialog.h"

#include <imgui_stdlib.h>

bool pathEditor(std::string& value) {
    bool changed{ false };

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 39);
    changed |= ImGui::InputText("##input", &value);
    ImGui::SameLine();

    if (ImGui::Button("...")) {
        DialogDesc dialogDesc{
            .folderDialog = true
        };
        std::filesystem::path outPath;
        openDialog(dialogDesc, outPath);
        if (changed |= !outPath.empty())
            value = outPath.string();
    }

    return changed;
}

bool settingEditor(const char* name, bool (*func)()) {
    bool changed{ false };
    ImGui::PushID(name);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s:", name);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    changed |= func();

    ImGui::PopID();

    return changed;
}

static const char* statusMessage{ "" };
static CURLcode statusCode{ CURLE_OK };

inline void renderSettingsTab() {
    auto* settings = Settings::instance;
    auto* modLoaderHandler = ModLoaderHandler::instance;
    bool changed{ false };
    bool requiresModReload{ false };

    ImGui::SeparatorText(translate("settings-tab_modmanager-sectionname"));
    requiresModReload |= changed |= settingEditor(translate("settings-tab_modmanager-mods_folder"), [](){
        return pathEditor(Settings::instance->modsFolderPath);
    });
    changed |= settingEditor(translate("settings-tab_modmanager-game_folder"), [](){
        return pathEditor(Settings::instance->gameFolderPath);
    });
    changed |= settingEditor(translate("settings-tab_modmanager-language"), []() {
    auto* settings = Settings::instance;
        bool changed{ false };
        if (ImGui::BeginCombo("##settings-tab_language-name", getLanguageName(settings->languageName.c_str()))) {
            auto it = langNameHashMap.begin();
            for (size_t x = 0; x < langNameHashMap.size(); x++) {
                bool selected{ it->first == settings->languageName };
                if (changed |= ImGui::Selectable(it->second, &selected, 0, { 0, ImGui::CalcTextSize(it->second).y*1.2f }))
                    settings->languageName = it->first;

                std::advance(it, 1);
            }

            ImGui::EndCombo();
        }
        return changed;
    });

    ImGui::SeparatorText(translate("settings-tab_modloader-sectionname"));

    ImGui::BeginDisabled(modLoaderHandler->fetching || modLoaderHandler->downloading);
    if (ImGui::Button(translate("settings-tab_modloader-fetch"))) {
        modLoaderHandler->fetch([](CURLcode code) {
            statusCode = code;
            if (code != CURLE_OK) {
                statusMessage = translate("settings-tab_modloader-fetch_fail");
                return;
            }

            auto* modLoaderHandler = ModLoaderHandler::instance;
            int64_t downloadedId = modLoaderHandler->getDownloadedReleaseId();
            if (modLoaderHandler->latestReleaseInfo.id == downloadedId)
                statusMessage = translate("settings-tab_modloader-fetch_exists");
            else if (downloadedId == -1)
                statusMessage = translate("settings-tab_modloader-fetch_noloader");
            else
                statusMessage = translate("settings-tab_modloader-fetch_newver");
        });
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    ImGui::BeginDisabled(
        !std::filesystem::exists(settings->gameFolderPath) || 
        modLoaderHandler->latestReleaseInfo.id == -1 || 
        modLoaderHandler->fetching || 
        modLoaderHandler->downloading
    );
    if (ImGui::Button(translate("settings-tab_modloader-download"))) {
        modLoaderHandler->download([](CURLcode code, std::vector<char>& data){
            statusCode = code;
            if (code != CURLE_OK) {
                statusMessage = translate("settings-tab_modloader-download_fail");
                return;
            }

            auto* settings = Settings::instance;
            auto* modLoaderHandler = ModLoaderHandler::instance;
            std::filesystem::path modLoaderPath{ settings->gameFolderPath };
            modLoaderPath /= "d3d8.dll";

            FILE* file = fopen(modLoaderPath.string().c_str(), "wb");
            fwrite(data.data(), data.size(), 1, file);
            fclose(file);

            modLoaderHandler->setDownloadedReleaseId(modLoaderHandler->latestReleaseInfo.id);

            statusMessage = translate("settings-tab_modloader-download_success");
        });
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    if (modLoaderHandler->getDownloadedReleaseId() == -1) {
        ImGui::Text(translate("settings-tab_modloader-fetch_noloader"));
    }
    else {
        ImGui::Text("%s: %ld", translate("settings-tab_modloader-downloadedver"), modLoaderHandler->getDownloadedReleaseId());
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10, 10 });
        if (ImGui::BeginPopupContextItem("downloadedver-popup")) {
            if (ImGui::Selectable(translate("settings-tab_modloader-downloadedver_refresh"))) {
                modLoaderHandler->refreshDownloadedReleaseId();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }

    if (statusCode != CURLE_OK)
        ImGui::Text("%s: %s %s", translate("settings-tab_modloader-status"), statusMessage, curl_easy_strerror(statusCode));
    else
        ImGui::Text("%s: %s", translate("settings-tab_modloader-status"), statusMessage);

    if (changed)
        settings->serialize();

    if (requiresModReload) {
        auto* app = Application::instance;
        app->modsFolder = settings->modsFolderPath;
        app->reloadMods(app->modsFolder);
    }
}
