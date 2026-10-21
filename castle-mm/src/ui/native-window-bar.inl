#include "graphics/graphics.h"

bool draggingWindow{ false };
double startMouseX = 0.0;
double startMouseY = 0.0;
int startWindowX = 0;
int startWindowY = 0;

inline void renderNativeWindowBar() {
    auto* graphics = Graphics::instance;
    auto* window = graphics->window;
    if (!graphics->nativeWindowBar) {
        if (ImGui::BeginMainMenuBar()) {
            ImGui::Text("Castle Mod Manager");
            ImGui::SameLine();

            float menuBarWidth = ImGui::GetContentRegionAvail().x - 40 * 3;
            ImGui::Dummy({ menuBarWidth, 0 });
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.4f, 0.4f, 0.4f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2f, 0.2f, 0.2f, 1.0f });
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

            if (ImGui::Button("-")) {
                glfwIconifyWindow(graphics->window);
            }
            ImGui::SameLine();

            if (ImGui::Button("O")) {
                if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED))
                    glfwRestoreWindow(window);
                else
                    glfwMaximizeWindow(window);
            }
            ImGui::SameLine();

            if (ImGui::Button("X")) {
                glfwSetWindowShouldClose(window, true);
            }

            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(3);

            bool canDrag = ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered();

            if (canDrag && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !draggingWindow) {
                glfwGetCursorPos(window, &startMouseX, &startMouseY);
                draggingWindow = true;
            }

            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                draggingWindow = false;

            if (draggingWindow) {
                double mouseX, mouseY;
                glfwGetCursorPos(window, &mouseX, &mouseY);
                glfwGetWindowPos(window, &startWindowX, &startWindowY);

                int newX = startWindowX - startMouseX + (int)mouseX;
                int newY = startWindowY - startMouseY + (int)mouseY;

                glfwSetWindowPos(window, newX, newY);
            }

            ImGui::EndMainMenuBar();
        }

        ImGui::Dummy({ ImGui::GetWindowWidth(), 20 });
    }
}
