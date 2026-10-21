#include "graphics/graphics.h"
#include "../app.h"

// This is so dirty, I don't even know how I came up with all this "inbetween line" crap.

inline void renderInbetweenRow(int row, const ImVec2& inbetween, int& reorderFrom, int& reorderTo) {
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 4, 0 });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 4, 0 });
    ImGui::PushStyleColor(ImGuiCol_Header, { 0, 0, 0, 0 });
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, { 0, 0, 0, 0 });
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, { 0, 0, 0, 0 });

    ImGui::Selectable(
        "##inbetweenrow",
        true,
        ImGuiSelectableFlags_SpanAllColumns |
        ImGuiSelectableFlags_AllowOverlap,
        { 0.0f, 8.0f }
    );

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);

    ImGui::PushStyleColor(ImGuiCol_DragDropTarget, { 0, 0, 0, 0 });
    if (ImGui::BeginDragDropTarget()) {
        constexpr float height = 5;
        ImGui::GetForegroundDrawList()->AddRectFilled({ inbetween.x - 5, inbetween.y + height }, { inbetween.x + ImGui::GetWindowWidth() - 5, inbetween.y + 8 - height }, ImGui::GetColorU32(ImGuiCol_Text));

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ROW_INDEX")) {
            reorderFrom = *(const int*)payload->Data;
            reorderTo = row;
        }

        ImGui::EndDragDropTarget();
    }
    ImGui::PopStyleColor();
}

const float buttonModsHeight{ 50 };

inline void renderModsTab() {
    int reorderFrom = -1;
    int reorderTo = -1;

    auto* app = Application::instance;
    auto& mods = app->mods;

    // Last "inbetween line" selectable adds in space, what do we do to remove the scroll? 
    // We add in a new child and make that end before the selectable, whilst the main child is unscrollable.. Genius. (Not really.)
    ImVec2 tableSize{ ImGui::GetWindowSize() };
    tableSize.y -= buttonModsHeight + padding;
    ImGui::BeginChild("mods-table-child", tableSize);
        
    int row = 0;
    ImVec2 lastScreenCursorPos{};

    if (ImGui::BeginTable("mods-table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_BordersOuter, tableSize)) {
        ImGui::TableSetupColumn(translate("mods-tab_name"));
        ImGui::TableSetupColumn(translate("mods-tab_version"));
        ImGui::TableSetupColumn(translate("mods-tab_author"));

        ImGui::TableHeadersRow();

        for (auto& mod : mods) {
            ImGui::PushID(mod.id.c_str());

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImVec2 preInbetween = ImGui::GetCursorPos();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
            ImVec2 inbetween = ImGui::GetCursorScreenPos();

            renderInbetweenRow(row, inbetween, reorderFrom, reorderTo);

            ImGui::SetCursorPos(preInbetween);
            ImVec2 startPos = ImGui::GetCursorPos();

            ImGui::Checkbox("##enabled", &mod.isEnabled);
            ImGui::SameLine();
            ImGui::Text(mod.name.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text(mod.version.c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text(mod.author.c_str());

            ImGui::SetCursorPos(startPos);

            ImGui::Selectable(
                "##row",
                false,
                ImGuiSelectableFlags_SpanAllColumns |
                ImGuiSelectableFlags_AllowOverlap,
                { ImGui::GetWindowWidth(), ImGui::GetFrameHeight() }
            );

            if (ImGui::BeginDragDropSource()) {
                ImGui::SetDragDropPayload("ROW_INDEX", &row, sizeof(int));
                ImGui::Text("%s", mod.name.c_str());

                ImGui::EndDragDropSource();
            }

            if (row == (int)mods.size() - 1) {
                ImGui::TableSetColumnIndex(0);
                lastScreenCursorPos = ImGui::GetCursorScreenPos();
            }

            ImGui::PopID();
            row++;
        }

        ImGui::EndTable();
    }

    ImGui::EndChild();

    ImVec2 preInbetween = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(lastScreenCursorPos);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25);
    ImVec2 inbetween = ImGui::GetCursorScreenPos();
    ImGui::PushID("bottomRow");
    renderInbetweenRow(row, inbetween, reorderFrom, reorderTo);
    ImGui::PopID();
    ImGui::SetCursorScreenPos(preInbetween);

    if (reorderFrom != -1 && 
        reorderTo != -1 && 
        reorderFrom != reorderTo) 
    {
        auto row = mods[reorderFrom];

        mods.erase(mods.begin() + reorderFrom);

        if (reorderFrom < reorderTo)
            --reorderTo;

        mods.insert(mods.begin() + reorderTo, row);
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);

    ImGui::BeginChild("mods-tab-buttons", { ImGui::GetWindowViewport()->Size.x, buttonModsHeight - padding });

    if (ImGui::Button(translate("mods-tab_refresh-list"), { ImGui::GetWindowViewport()->Size.x / 3 - padding, ImGui::GetContentRegionAvail().y }))
        app->reloadMods(app->modsFolder);

    ImGui::EndChild();
}
