#pragma once

// GLm
#include <glm/vec2.hpp>

// ImGui
#include <imgui.h>

// C++ STL
#include <functional>

namespace HG::Rendering::Base
{
    class Texture;
}

namespace HG::Editor
{
    struct Shortcut;
}

namespace ImGui
{
    // Widgets: Icon Selectable
    IMGUI_API bool IconSelectable(const char* label,bool selected, int flags, const ImVec2& size_arg={0, 0}, HG::Rendering::Base::Texture* icon=nullptr, const ImVec2& uv_tl={0, 0}, const ImVec2& uv_br={0, 0});

    // Widgets: Icon Trees
    IMGUI_API bool IconTreeNode   (const char* label,  HG::Rendering::Base::Texture* icon=nullptr, const ImVec2& uv_tl={0, 0}, const ImVec2& uv_br={0, 0});
    IMGUI_API bool IconTreeNode   (const char* str_id, HG::Rendering::Base::Texture* icon,         const ImVec2& uv_tl,        const ImVec2& uv_br, const char* fmt, ...) IM_FMTARGS(5);
    IMGUI_API bool IconTreeNode   (const void* ptr_id, HG::Rendering::Base::Texture* icon,         const ImVec2& uv_tl,        const ImVec2& uv_br, const char* fmt, ...) IM_FMTARGS(5);
    IMGUI_API bool IconTreeNodeV  (const char* str_id, HG::Rendering::Base::Texture* icon,         const ImVec2& uv_tl,        const ImVec2& uv_br, const char* fmt, va_list args) IM_FMTLIST(5);
    IMGUI_API bool IconTreeNodeV  (const void* ptr_id, HG::Rendering::Base::Texture* icon,         const ImVec2& uv_tl,        const ImVec2& uv_br, const char* fmt, va_list args) IM_FMTLIST(5);

    IMGUI_API bool IconTreeNodeEx (const char* label,  ImGuiTreeNodeFlags flags, HG::Rendering::Base::Texture* icon, const ImVec2& uv_tl, const ImVec2& uv_br);
    IMGUI_API bool IconTreeNodeEx (const char* str_id, ImGuiTreeNodeFlags flags, HG::Rendering::Base::Texture* icon, const ImVec2& uv_tl, const ImVec2& uv_br, const char* fmt, ...) IM_FMTARGS(6);
    IMGUI_API bool IconTreeNodeEx (const void* ptr_id, ImGuiTreeNodeFlags flags, HG::Rendering::Base::Texture* icon, const ImVec2& uv_tl, const ImVec2& uv_br, const char* fmt, ...) IM_FMTARGS(6);
    IMGUI_API bool IconTreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, HG::Rendering::Base::Texture* icon, const ImVec2& uv_tl, const ImVec2& uv_br, const char* fmt, va_list args) IM_FMTLIST(6);
    IMGUI_API bool IconTreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, HG::Rendering::Base::Texture* icon, const ImVec2& uv_tl, const ImVec2& uv_br, const char* fmt, va_list args) IM_FMTLIST(6);

    IMGUI_API bool IconTreeNodeBehaviour(ImGuiID id, HG::Rendering::Base::Texture *icon, const ImVec2& uv_tl, const ImVec2& uv_br, ImGuiTreeNodeFlags flags, const char* label, const char* label_end);

    IMGUI_API bool BeginDragDropTargetWindow();

    /**
     * @brief Translates from GLM vec2 to imgui vec2.
     * @param vec Vector object.
     * @return ImGui vector object.
     */
    ImVec2 fromGLM(const glm::vec2& vec);

    /**
     * @brief Class, that performs pushing and popping ImGui
     * ID with RAII.
     */
    class IDGuard
    {
    public:
        explicit IDGuard(int id);

        explicit IDGuard(void* id);

        explicit IDGuard(const char* id);

        explicit IDGuard(const char* begin_id, const char* end_id);

        ~IDGuard();
    };

    /**
     * @brief Class, that performs pushing and popping ImGui
     * disabled state and style with RAII.
     */
    class DisabledGuard
    {
    public:
        DisabledGuard();

        ~DisabledGuard();
    };

    /**
     * @brief Class, that describes some action.
     */
    class Item
    {
    public:

        using Callback = std::function<void()>;

        /**
         * @brief Constructor.
         */
        Item(const char* name, const char* id, HG::Editor::Shortcut* shortcut=nullptr);

        /**
         * @brief Destructor.
         */
        virtual ~Item();

        /**
         * @brief Method for setting callback
         * for item.
         * @param cb Callback.
         */
        void setCallback(Callback cb);

        /**
         * @brief Method for getting callback for this item.
         * @return Callback.
         */
        Callback callback() const;

        /**
         * @brief Method for getting item name.
         * @return Pointer to cstring name.
         */
        const char* name() const;

        /**
         * @brief Method for getting item id.
         * @return Pointer to cstring id.
         */
        const char* id() const;

        /**
         * @brief Method for getting item shortcut.
         * @return Pointer to item shortcut.
         */
        HG::Editor::Shortcut* shortcut() const;

    private:

        Callback m_callback;

        const char* m_name;
        const char* m_id;

        HG::Editor::Shortcut* m_shortcut;
    };

    /**
     * @brief Class, that describes some menu.
     */
    class Menu : public Item
    {
    public:


        struct Entry
        {
            enum class Type
            {
                Item,
                Menu,
                Separator
            };

            Entry(Type type, Item* item) :
                type(type),
                item(item)
            {}

            Type type;
            Item* item;
        };

        /**
         * @brief Constructor.
         */
        Menu();

        /**
         * @brief Constructor.
         * @param name Menu name.
         * @param id Menu id.
         */
        Menu(const char* name, const char* id);

        /**
         * @brief Destructor.
         */
        ~Menu() override;

        /**
         * @brief Method, that performs adding item
         * to menu.
         * @param name Item name.
         * @param id Item id.
         * @return Pointer to action for this item.
         */
        Item* addItem(const char* name, const char* id, HG::Editor::Shortcut* shortcut=nullptr);

        /**
         * @brief Method, that performs adding item that
         * opens internal menu.
         * @param name Item name.
         * @param id Item id.
         * @return Pointer to internal menu.
         */
        Menu* addMenu(const char* name, const char* id);

        /**
         * @brief Method for adding separator.
         */
        void addSeparator();

        /**
         * @brief Method for getting menu entries.
         */
        const std::vector<Entry> entries() const;

    private:

        std::vector<Entry> m_entries;
    };

    /**
     * @brief Class, that performs menu rendering
     * as main menu.
     */
    class MainMenuRenderer
    {
    public:

        /**
         * @brief Constructor.
         */
        MainMenuRenderer();

        /**
         * @brief Method for rendering menu.
         * @param menu Reference to menu object.
         */
        void render(const Menu& menu);

    private:

        void renderMenu(const Menu& menu);

        void renderItem(const Item& item);

        void renderSeparator();
    };

    /**
     * @brief Class, that performs menu rendering
     * as context menu.
     */
    class ContextMenuRenderer
    {
    public:

        /**
         * @brief Constructor.
         */
        ContextMenuRenderer();

        /**
         * @brief Method for rendering menu.
         * @param menu Reference to menu object.
         */
        void render(const Menu& menu);

    private:

        void renderMenu(const Menu& menu);

        void renderItem(const Item& item);

        void renderSeparator();
    };
}

ImVec2 operator+(const ImVec2& l, const ImVec2& r);
ImVec2 operator-(const ImVec2& l, const ImVec2& r);