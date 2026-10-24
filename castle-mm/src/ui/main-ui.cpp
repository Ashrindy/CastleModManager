#include "main-ui.h"
#include "graphics/graphics.h"
#include "../app.h"
#include "native-window-bar.inl"
#include "mods-tab.inl"
#include "settings-tab.inl"

void renderContentFrame(void (*func)()) {
    auto* windowViewport = ImGui::GetWindowViewport();
    ImVec2 contentSize = windowViewport->Size;
    contentSize.y -= (ImGui::GetCursorPosY() + buttonHeight + 5);
    contentSize -= ImVec2{ padding * 2, padding * 2 };
    
    ImGui::SetCursorPos({ ImGui::GetCursorPosX() + padding, ImGui::GetCursorPosY() + padding });

    ImGui::BeginChild("content", contentSize, 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    func();
    ImGui::EndChild();
}

inline void renderTab(const char* name, void (*func)()) {
    if (ImGui::BeginTabItem(name)) {
        renderContentFrame(func);
        ImGui::EndTabItem();
    }
}

inline void renderButtons() {
    auto* app = Application::instance;

#ifdef ENABLE_FLATPAK
    static constexpr int buttonCount{ 1 };
#else
    static constexpr int buttonCount{ 2 };
#endif

    auto* windowViewport = ImGui::GetWindowViewport();
    ImGui::SetCursorPos({ ImGui::GetCursorPosX() + padding, ImGui::GetCursorPosY() + padding });

    ImGui::PushFont(Graphics::instance->largerFont);

    if (ImGui::Button(translate("save-button-name"), { windowViewport->Size.x / buttonCount - padding * buttonCount , buttonHeight - padding }))
        app->saveMods(app->modsFolder);

    // TODO: Figure out how to launch the game in flatpak.
#ifndef ENABLE_FLATPAK
    ImGui::SameLine();
    ImGui::SetCursorPos({ ImGui::GetCursorPosX() + padding, ImGui::GetCursorPosY() });

    if (ImGui::Button(translate("save-and-play-button-name"), { windowViewport->Size.x / buttonCount - padding * buttonCount, buttonHeight - padding })) {
        app->saveMods(app->modsFolder);
        app->launchGame(std::filesystem::path{ Settings::instance->gameFolderPath } / "CastleStrike.exe");
    }
#endif

    ImGui::PopFont();
}

void renderUI() {
    ImGuiViewport* imguiViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(imguiViewport->Pos);
    ImGui::SetNextWindowSize(imguiViewport->Size);
    ImGui::SetNextWindowViewport(imguiViewport->ID);

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoDocking  | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove     | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

    ImGui::Begin("MainWindow", nullptr, windowFlags);

    renderNativeWindowBar();

    auto beforeVersionPos = ImGui::GetCursorPos();
    auto versionTexSize = ImGui::CalcTextSize(version);
    ImGui::SetCursorPos({ ImGui::GetWindowWidth() - (versionTexSize.x + padding), ImGui::GetCursorPosY() + 2 });
    ImGui::Text(version);
    ImGui::SetCursorPos(beforeVersionPos);

    ImGui::BeginTabBar("MainTab");
    
    renderTab(translate("mods-tab-name"), renderModsTab);
    renderTab(translate("settings-tab-name"), renderSettingsTab);

    // Manually draw a Separator to not take up space.
    ImVec2 dividerLineStart{ ImGui::GetCursorPos() };
    dividerLineStart.y -= 0.5f;
    ImVec2 dividerLineEnd{ dividerLineStart };
    dividerLineEnd.x += ImGui::GetWindowWidth();
    ImGui::GetWindowDrawList()->AddLine(dividerLineStart, dividerLineEnd, ImGui::GetColorU32(ImGuiCol_Separator));

    renderButtons();

    ImGui::EndTabBar();

    ImGui::End();

    ImGui::PopStyleVar(3);
}
