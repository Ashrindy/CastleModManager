#include "mod-details.h"
#include "editors.h"

#include <imgui_stdlib.h>

bool shouldRenderModDetails{ false };
Mod* modDetails{ nullptr };
bool windowSizeSet{ false };

void setModDetails(Mod* mod) {
    shouldRenderModDetails = true;
    modDetails = mod;
}

void renderModDetails() {
    if (!shouldRenderModDetails && !modDetails) return;

    auto* app = Application::instance;

    if (!windowSizeSet) {
        windowSizeSet = true;
        auto windowSize = ImGui::GetMainViewport()->Size;
        ImGui::SetNextWindowSize({ windowSize.x * 0.8f, windowSize.y * 0.8f });
    }

    bool changed{ false };

    constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
#ifndef __WIN32
    windowFlags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
#endif

    int dllFileToRemove{ -1 };
    if (ImGui::Begin(translate("mod-details_name"), &shouldRenderModDetails, windowFlags)) {
        ImGui::SeparatorText(translate("general_details"));
        changed |= settingEditor(translate("general_name"), modDetails->name);
        changed |= settingEditor(translate("general_version"), modDetails->version);
        changed |= settingEditor(translate("general_author"), modDetails->author);

        ImGui::SeparatorText(translate("general_main"));
        if (ImGui::BeginListBox(translate("mod-details_dllfiles"))) {
            int row = 0;
            for (auto& file : modDetails->dllFiles) {
                ImGui::PushID(row);

                changed |= ImGui::InputText("##x", &file);
                ImGui::SameLine();
                if (ImGui::Button(translate("general_remove")))
                    dllFileToRemove = row;

                ImGui::PopID();
                row++;
            }

            if (changed |= ImGui::Button(translate("general_add")))
                modDetails->dllFiles.emplace_back();

            ImGui::EndListBox();
        }
    }
    ImGui::End();

    if (dllFileToRemove != -1) {
        changed = true;
        modDetails->dllFiles.erase(modDetails->dllFiles.begin() + dllFileToRemove);
    }

    if (!shouldRenderModDetails) {
        modDetails = nullptr;
        windowSizeSet = false;
    }

    if (changed)
        app->saveMod(*modDetails);
}
