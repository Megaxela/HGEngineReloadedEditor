#pragma once

// ImGui
#include <imgui.h>

// C++ STL
#include <functional>

namespace HG::Rendering::Base
{
    class Texture;
}

namespace ImGui
{
    bool IconSelectable(const char* label, bool selected, int flags, const ImVec2& size_arg={0, 0}, HG::Rendering::Base::Texture* icon=nullptr);

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
        Item(const char* name, const char* id);

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

        const char* name() const;

        const char* id() const;

    private:

        Callback m_callback;

        const char* m_name;
        const char* m_id;
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
        Item* addItem(const char* name, const char* id);

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

    class ContextMenuRenderer
    {
    public:

        ContextMenuRenderer();

        void render(const Menu& menu);

    private:
        void renderMenu(const Menu& menu);

        void renderItem(const Item& item);

        void renderSeparator();
    };
}

ImVec2 operator+(const ImVec2& l, const ImVec2& r);
ImVec2 operator-(const ImVec2& l, const ImVec2& r);