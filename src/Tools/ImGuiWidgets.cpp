// Editor
#include <Tools/ImGuiWidgets.hpp>
#include <Editor/ShortcutsProcessor.hpp>

// ALogger
#include <CurrentLogger.hpp>

// ImGui
#include <imgui_internal.h>

ImVec2 operator+(const ImVec2& l, const ImVec2& r)
{
    auto result = l;

    result.x += r.x;
    result.y += r.y;

    return result;
}

ImVec2 operator-(const ImVec2& l, const ImVec2& r)
{
    auto result = l;

    result.x -= r.x;
    result.y -= r.y;

    return result;
}

ImVec2 ImGui::fromGLM(const glm::vec2 &vec)
{
    return {vec.x, vec.y};
}

bool ImGui::IconSelectable(const char *label,
                           bool selected,
                           int flags,
                           const ImVec2& size_arg,
                           HG::Rendering::Base::Texture *icon,
                           const ImVec2& uv_tl,
                           const ImVec2& uv_br)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet) // FIXME-OPT: Avoid if vertically clipped.
        ImGui::PopClipRect();

    ImGuiID id = window->GetID(label);
    ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrentLineTextBaseOffset;
    ImRect bb_inner(pos + ImVec2(style.ItemInnerSpacing.x, 0), pos + size);
    bb_inner.Min = bb_inner.Min + ImVec2(size.y, 0);

    ImRect item_size(pos, pos + size);
    ImGui::ItemSize(item_size);

    // Fill horizontal space.
    ImVec2 window_padding = window->WindowPadding;
    float max_x = (flags & ImGuiSelectableFlags_SpanAllColumns) ? ImGui::GetWindowContentRegionMax().x : ImGui::GetContentRegionMax().x;
    float w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x - window->DC.CursorPos.x);
    ImVec2 size_draw((size_arg.x != 0 && !(flags & ImGuiSelectableFlags_DrawFillAvailWidth)) ? size_arg.x : w_draw, size_arg.y != 0.0f ? size_arg.y : size.y);
    ImRect bb(pos, pos + size_draw);
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_DrawFillAvailWidth))
        bb.Max.x += window_padding.x;

    // Selectables are tightly packed together, we extend the box to cover spacing between selectable.
    float spacing_L = (float)(int)(style.ItemSpacing.x * 0.5f);
    float spacing_U = (float)(int)(style.ItemSpacing.y * 0.5f);
    float spacing_R = style.ItemSpacing.x - spacing_L;
    float spacing_D = style.ItemSpacing.y - spacing_U;
    bb.Min.x -= spacing_L;
    bb.Min.y -= spacing_U;
    bb.Max.x += spacing_R;
    bb.Max.y += spacing_D;
    if (!ImGui::ItemAdd(bb, (flags & ImGuiSelectableFlags_Disabled) ? 0 : id))
    {
        if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
            ImGui::PushColumnClipRect();
        return false;
    }

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_NoHoldingActiveID) button_flags |= ImGuiButtonFlags_NoHoldingActiveID;
    if (flags & ImGuiSelectableFlags_PressedOnClick) button_flags |= ImGuiButtonFlags_PressedOnClick;
    if (flags & ImGuiSelectableFlags_PressedOnRelease) button_flags |= ImGuiButtonFlags_PressedOnRelease;
    if (flags & ImGuiSelectableFlags_Disabled) button_flags |= ImGuiButtonFlags_Disabled;
    if (flags & ImGuiSelectableFlags_AllowDoubleClick) button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);
    if (flags & ImGuiSelectableFlags_Disabled)
        selected = false;

    // Hovering selectable with mouse updates NavId accordingly so navigation can be resumed with gamepad/keyboard (this doesn't happen on most widgets)
    if (pressed || hovered)
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            ImGui::SetNavID(id, window->DC.NavLayerCurrent);
        }
    if (pressed)
        ImGui::MarkItemEdited(id);

    // Render
    if (hovered || selected)
    {
        const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        ImGui::RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
        ImGui::RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);
    }

    if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
    {
        ImGui::PushColumnClipRect();
        bb.Max.x -= (ImGui::GetContentRegionMax().x - max_x);
    }

    if (flags & ImGuiSelectableFlags_Disabled) ImGui::PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
    ImGui::RenderTextClipped(bb_inner.Min, bb.Max, label, NULL, &label_size, ImVec2(0.0f,0.0f));
    if (flags & ImGuiSelectableFlags_Disabled) ImGui::PopStyleColor();

    if (icon != nullptr)
    {
        window->DrawList->AddImage(icon, pos, pos + ImVec2(size.y, size.y), uv_tl, uv_br);
    }

    // Automatically close popups
    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(window->DC.ItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
        ImGui::CloseCurrentPopup();
    return pressed;
}

bool ImGui::IconTreeNode(const char* label, HG::Rendering::Base::Texture* icon, const ImVec2& uv_tl, const ImVec2& uv_br)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    return IconTreeNodeBehaviour(window->GetID(label), icon, uv_tl, uv_br, 0, label, nullptr);
}

bool ImGui::IconTreeNode(const char* str_id,
                         HG::Rendering::Base::Texture* icon,
                         const ImVec2& uv_tl,
                         const ImVec2& uv_br,
                         const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = IconTreeNodeV(str_id, icon, uv_tl, uv_br, fmt, args);
    va_end(args);
    return is_open;
}

bool ImGui::IconTreeNode(const void* ptr_id,
                         HG::Rendering::Base::Texture* icon,
                         const ImVec2& uv_tl,
                         const ImVec2& uv_br,
                         const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = IconTreeNodeV(ptr_id, icon, uv_tl, uv_br, fmt, args);
    va_end(args);
    return is_open;
}

bool ImGui::IconTreeNodeV(const char* str_id,
                          HG::Rendering::Base::Texture* icon,
                          const ImVec2& uv_tl,
                          const ImVec2& uv_br,
                          const char* fmt,
                          va_list args)
{
    return IconTreeNodeExV(str_id, 0, icon, uv_tl, uv_br, fmt, args);
}

bool ImGui::IconTreeNodeV(const void* ptr_id,
                          HG::Rendering::Base::Texture* icon,
                          const ImVec2& uv_tl,
                          const ImVec2& uv_br,
                          const char* fmt,
                          va_list args)
{
    return IconTreeNodeExV(ptr_id, 0, icon, uv_tl, uv_br, fmt, args);
}

bool ImGui::IconTreeNodeEx(const char* label,
                           ImGuiTreeNodeFlags flags,
                           HG::Rendering::Base::Texture* icon,
                           const ImVec2& uv_tl,
                           const ImVec2& uv_br)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    return IconTreeNodeBehaviour(window->GetID(label), icon, uv_tl, uv_br, flags, label, nullptr);
}

bool ImGui::IconTreeNodeEx(const char* str_id,
                           ImGuiTreeNodeFlags flags,
                           HG::Rendering::Base::Texture* icon,
                           const ImVec2& uv_tl,
                           const ImVec2& uv_br,
                           const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = IconTreeNodeExV(str_id, flags, icon, uv_tl, uv_br, fmt, args);
    va_end(args);
    return is_open;
}

bool ImGui::IconTreeNodeEx(const void* ptr_id,
                           ImGuiTreeNodeFlags flags,
                           HG::Rendering::Base::Texture* icon,
                           const ImVec2& uv_tl,
                           const ImVec2& uv_br,
                           const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = IconTreeNodeExV(ptr_id, flags, icon, uv_tl, uv_br, fmt, args);
    va_end(args);
    return is_open;
}

bool ImGui::IconTreeNodeExV(const char* str_id,
                            ImGuiTreeNodeFlags flags,
                            HG::Rendering::Base::Texture* icon,
                            const ImVec2& uv_tl,
                            const ImVec2& uv_br,
                            const char* fmt,
                            va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const char* label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    return IconTreeNodeBehaviour(window->GetID(str_id), icon, uv_tl, uv_br, flags, g.TempBuffer, label_end);
}

bool ImGui::IconTreeNodeExV(const void* ptr_id,
                            ImGuiTreeNodeFlags flags,
                            HG::Rendering::Base::Texture* icon,
                            const ImVec2& uv_tl,
                            const ImVec2& uv_br,
                            const char* fmt,
                            va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const char* label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    return IconTreeNodeBehaviour(window->GetID(ptr_id), icon, uv_tl, uv_br, flags, g.TempBuffer, label_end);
}

bool ImGui::IconTreeNodeBehaviour(ImGuiID id,
                                  HG::Rendering::Base::Texture *icon,
                                  const ImVec2& uv_tl,
                                  const ImVec2& uv_br,
                                  ImGuiTreeNodeFlags flags,
                                  const char* label,
                                  const char* label_end)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    const ImVec2 padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, 0.0f);

    if (!label_end)
        label_end = FindRenderedTextEnd(label);
    const ImVec2 label_size = CalcTextSize(label, label_end, false);

    // We vertically grow up to current line height up the typical widget height.
    // todo: Remove magic numbers 15 (it's icon width/height), 1.8 (experimental offset)
    const float text_base_offset_y = ImMax(1.8f, ImMax(padding.y, window->DC.CurrentLineTextBaseOffset)); // Latch before ItemSize changes it
    const float frame_height = ImMax(15.0f, ImMax(ImMin(window->DC.CurrentLineSize.y, g.FontSize + style.FramePadding.y*2), label_size.y + padding.y*2));
    ImRect frame_bb = ImRect(window->DC.CursorPos, ImVec2(window->Pos.x + GetContentRegionMax().x, window->DC.CursorPos.y + frame_height));
    if (display_frame)
    {
        // Framed header expand a little outside the default padding
        frame_bb.Min.x -= (float)(int)(window->WindowPadding.x*0.5f) - 1;
        frame_bb.Max.x += (float)(int)(window->WindowPadding.x*0.5f) - 1;
    }

    const float text_offset_x = (g.FontSize*2 + (display_frame ? padding.x*3 : padding.x*2));   // Collapser arrow width + Spacing + Image + Spacing
    const float image_offset_x = g.FontSize + (display_frame ? padding.x * 2 : padding.x * 1);
    const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x*2 : 0.0f);   // Include collapser
    ItemSize(ImVec2(text_width, frame_height), text_base_offset_y);

    // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
    // (Ideally we'd want to add a flag for the user to specify if we want the hit test to be done up to the right side of the content or not)
    const ImRect interact_bb = display_frame ? frame_bb : ImRect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + text_width + style.ItemSpacing.x*2, frame_bb.Max.y);
    bool is_open = TreeNodeBehaviorIsOpen(id, flags);

    // Store a flag for the current depth to tell if we will allow closing this node when navigating one of its child.
    // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
    // This is currently only support 32 level deep and we are fine with (1 << Depth) overflowing into a zero.
    if (is_open && !g.NavIdIsAlive && (flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        window->DC.TreeDepthMayJumpToParentOnPop |= (1 << window->DC.TreeDepth);

    bool item_add = ItemAdd(interact_bb, id);
    window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
    window->DC.LastItemDisplayRect = frame_bb;

    if (!item_add)
    {
        if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
            TreePushRawID(id);
        return is_open;
    }

    // Flags that affects opening behavior:
    // - 0(default) ..................... single-click anywhere to open
    // - OpenOnDoubleClick .............. double-click anywhere to open
    // - OpenOnArrow .................... single-click on arrow to open
    // - OpenOnDoubleClick|OpenOnArrow .. single-click on arrow or double-click anywhere to open
    ImGuiButtonFlags button_flags = ImGuiButtonFlags_NoKeyModifiers | ((flags & ImGuiTreeNodeFlags_AllowItemOverlap) ? ImGuiButtonFlags_AllowItemOverlap : 0);
    if (!(flags & ImGuiTreeNodeFlags_Leaf))
        button_flags |= ImGuiButtonFlags_PressedOnDragDropHold;
    if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
        button_flags |= ImGuiButtonFlags_PressedOnDoubleClick | ((flags & ImGuiTreeNodeFlags_OpenOnArrow) ? ImGuiButtonFlags_PressedOnClickRelease : 0);

    bool hovered, held, pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    if (!(flags & ImGuiTreeNodeFlags_Leaf))
    {
        bool toggled = false;
        if (pressed)
        {
            toggled = !(flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) || (g.NavActivateId == id);
            if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
                toggled |= IsMouseHoveringRect(interact_bb.Min, ImVec2(interact_bb.Min.x + text_offset_x, interact_bb.Max.y)) && (!g.NavDisableMouseHover);
            if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
                toggled |= g.IO.MouseDoubleClicked[0];
            if (g.DragDropActive && is_open) // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but never close it again.
                toggled = false;
        }

        if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Left && is_open)
        {
            toggled = true;
            NavMoveRequestCancel();
        }
        if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Right && !is_open) // If there's something upcoming on the line we may want to give it the priority?
        {
            toggled = true;
            NavMoveRequestCancel();
        }

        if (toggled)
        {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(id, is_open);
        }
    }
    if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
        SetItemAllowOverlap();

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
    const ImVec2 text_pos = frame_bb.Min + ImVec2(text_offset_x, text_base_offset_y);
    const ImVec2 image_pos = frame_bb.Min + ImVec2(image_offset_x, 0);

    if (display_frame)
    {
        // Framed type
        RenderFrame(frame_bb.Min, frame_bb.Max, col, true, style.FrameRounding);
        RenderNavHighlight(frame_bb, id, ImGuiNavHighlightFlags_TypeThin);
        RenderArrow(frame_bb.Min + ImVec2(padding.x, text_base_offset_y), is_open ? ImGuiDir_Down : ImGuiDir_Right, 1.0f);

        if (icon != nullptr)
        {
            window->DrawList->AddImage(icon, image_pos, image_pos + ImVec2(frame_bb.GetHeight(), frame_bb.GetHeight()), uv_tl, uv_br);
        }

        if (g.LogEnabled)
        {
            // NB: '##' is normally used to hide text (as a library-wide feature), so we need to specify the text range to make sure the ## aren't stripped out here.
            const char log_prefix[] = "\n##";
            const char log_suffix[] = "##";
            LogRenderedText(&text_pos, log_prefix, log_prefix+3);
            RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
            LogRenderedText(&text_pos, log_suffix+1, log_suffix+3);
        }
        else
        {
            RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
        }
    }
    else
    {
        // Unframed typed for tree nodes
        if (hovered || (flags & ImGuiTreeNodeFlags_Selected))
        {
            RenderFrame(frame_bb.Min, frame_bb.Max, col, false);
            RenderNavHighlight(frame_bb, id, ImGuiNavHighlightFlags_TypeThin);
        }

        if (flags & ImGuiTreeNodeFlags_Bullet)
            RenderBullet(frame_bb.Min + ImVec2(text_offset_x * 0.5f, g.FontSize*0.50f + text_base_offset_y));
        else if (!(flags & ImGuiTreeNodeFlags_Leaf))
            RenderArrow(frame_bb.Min + ImVec2(padding.x, g.FontSize*0.15f + text_base_offset_y), is_open ? ImGuiDir_Down : ImGuiDir_Right, 0.70f);
        if (g.LogEnabled)
            LogRenderedText(&text_pos, ">");

        if (icon != nullptr)
        {
            window->DrawList->AddImage(icon, image_pos, image_pos + ImVec2(frame_bb.GetHeight(), frame_bb.GetHeight()), uv_tl, uv_br);
        }

        RenderText(text_pos, label, label_end, false);
    }

    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        TreePushRawID(id);
    return is_open;
}


ImGui::IDGuard::IDGuard(int id)
{
    ImGui::PushID(id);
}

ImGui::IDGuard::IDGuard(void *id)
{
    ImGui::PushID(id);
}

ImGui::IDGuard::IDGuard(const char *id)
{
    ImGui::PushID(id);
}

ImGui::IDGuard::IDGuard(const char *begin_id, const char *end_id)
{
    ImGui::PushID(begin_id, end_id);
}

ImGui::IDGuard::~IDGuard()
{
    ImGui::PopID();
}

ImGui::DisabledGuard::DisabledGuard()
{
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
}

ImGui::DisabledGuard::~DisabledGuard()
{
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
}



ImGui::Item::Item(const char *name, const char *id, HG::Editor::Shortcut* shortcut) :
    m_callback(),
    m_name(name),
    m_id(id),
    m_shortcut(shortcut)
{

}

ImGui::Item::~Item()
{

}

void ImGui::Item::setCallback(ImGui::Item::Callback cb)
{
    m_callback = std::move(cb);
}

ImGui::Item::Callback ImGui::Item::callback() const
{
    return m_callback;
}

const char *ImGui::Item::name() const
{
    return m_name;
}

const char *ImGui::Item::id() const
{
    return m_id;
}

HG::Editor::Shortcut* ImGui::Item::shortcut() const
{
    return m_shortcut;
}

ImGui::Menu::Menu() :
    Item(nullptr, nullptr),
    m_entries()
{

}

ImGui::Menu::Menu(const char *name, const char *id) :
    Item(name, id),
    m_entries()
{

}

ImGui::Menu::~Menu()
{
    for (auto& entry : m_entries)
    {
        delete entry.item;
    }
}

ImGui::Item *ImGui::Menu::addItem(const char *name, const char *id, HG::Editor::Shortcut* shortcut)
{
    auto item = new Item(name, id, shortcut);

    m_entries.emplace_back(Entry::Type::Item, item);

    if (shortcut)
    {
        auto previousCallback = shortcut->callback;

        shortcut->callback = [item, previousCallback]()
        {
            if (previousCallback)
            {
                previousCallback();
            }

            if (item->callback())
            {
                item->callback()();
            }
        };
    }

    return item;
}

ImGui::Menu *ImGui::Menu::addMenu(const char *name, const char *id)
{
    auto menu = new Menu(name, id);

    m_entries.emplace_back(Entry::Type::Menu, menu);

    return menu;
}

void ImGui::Menu::addSeparator()
{
    m_entries.emplace_back(Entry::Type::Separator, nullptr);
}

const std::vector<ImGui::Menu::Entry> ImGui::Menu::entries() const
{
    return m_entries;
}

ImGui::ContextMenuRenderer::ContextMenuRenderer()
{

}

void ImGui::ContextMenuRenderer::render(const ImGui::Menu &menu)
{
    if (ImGui::BeginPopupContextWindow(menu.id()))
    {
        renderMenu(menu);

        ImGui::EndPopup();
    }
}

void ImGui::ContextMenuRenderer::renderMenu(const ImGui::Menu &menu)
{
    for (const auto& entry : menu.entries())
    {
        switch (entry.type)
        {
        case Menu::Entry::Type::Item:
        {
            renderItem(*entry.item);
            break;
        }
        case Menu::Entry::Type::Menu:
        {
            ImGui::IDGuard idGuard(entry.item->id());
            if (ImGui::BeginMenu(entry.item->name()))
            {
                renderMenu(reinterpret_cast<const Menu &>(*entry.item));

                ImGui::EndMenu();
            }
            break;
        }
        case Menu::Entry::Type::Separator:
        {
            renderSeparator();
            break;
        }
        }
    }
}

void ImGui::ContextMenuRenderer::renderItem(const ImGui::Item &item)
{
    std::string name;

    if (item.shortcut())
    {
        name = item.shortcut()->generateName();
    }

    if (ImGui::IDGuard(item.id()),
        ImGui::MenuItem(item.name(), name.c_str()))
    {
        if (!item.callback())
            return;

        item.callback()();
    }
}

void ImGui::ContextMenuRenderer::renderSeparator()
{
    ImGui::Separator();
}

ImGui::MainMenuRenderer::MainMenuRenderer()
{

}

void ImGui::MainMenuRenderer::render(const ImGui::Menu& menu)
{
    ImGui::BeginMainMenuBar();

    renderMenu(menu);

    ImGui::EndMainMenuBar();
}

void ImGui::MainMenuRenderer::renderMenu(const ImGui::Menu& menu)
{
    for (const auto& entry : menu.entries())
    {
        switch (entry.type)
        {
        case Menu::Entry::Type::Item:
        {
            renderItem(*entry.item);
            break;
        }
        case Menu::Entry::Type::Menu:
        {
            ImGui::IDGuard idGuard(entry.item->id());
            if (ImGui::BeginMenu(entry.item->name()))
            {
                renderMenu(reinterpret_cast<const Menu &>(*entry.item));

                ImGui::EndMenu();
            }
            break;
        }
        case Menu::Entry::Type::Separator:
        {
            renderSeparator();
            break;
        }
        }
    }
}

void ImGui::MainMenuRenderer::renderItem(const ImGui::Item& item)
{
    std::string name;

    if (item.shortcut())
    {
        name = item.shortcut()->generateName();
    }

    if (ImGui::IDGuard(item.id()),
        ImGui::MenuItem(item.name(), name.c_str()))
    {
        if (!item.callback())
            return;

        item.callback()();
    }
}

void ImGui::MainMenuRenderer::renderSeparator()
{
    ImGui::Separator();
}
