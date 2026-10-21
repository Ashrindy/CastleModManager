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

inline void renderSettingsTab() {
    auto* settings = Settings::instance;
    bool changed{ false };
    bool requiresModReload{ false };
    requiresModReload |= changed |= settingEditor(translate("settings-tab_mods-folder"), [](){
        return pathEditor(Settings::instance->modsFolderPath);
    });
    changed |= settingEditor(translate("settings-tab_game-folder"), [](){
        return pathEditor(Settings::instance->gameFolderPath);
    });
    changed |= settingEditor(translate("settings-tab_language"), []() {
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

    if (changed)
        settings->serialize();

    if (requiresModReload) {
        auto* app = Application::instance;
        app->modsFolder = settings->modsFolderPath;
        app->reloadMods(app->modsFolder);
    }
}
