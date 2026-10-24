#pragma once
#include <imgui_stdlib.h>

template<typename T>
static bool settingEditor(const char* name, T&& func) {
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

inline static bool settingEditor(const char* name, std::string& value) {
    return settingEditor(name, [&value](){
        return ImGui::InputText("##x", &value);
    });
}
