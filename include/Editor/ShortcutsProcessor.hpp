#pragma once

// C++ STL
#include <unordered_set>

// HG::Core
#include <HG/Core/Input.hpp>

namespace HG::Editor
{
    struct Shortcut
    {
        std::unordered_set<HG::Core::Input::Keyboard::Key> keys;
        std::unordered_set<HG::Core::Input::Keyboard::Modifiers> modifiers;

        std::function<void()> callback;
        bool fired = false;
    };

    /**
     * @brief Class, that describes processor
     * for shortcuts.
     */
    class ShortcutsProcessor
    {
    public:

        /**
         * @brief Constructor.
         */
        ShortcutsProcessor();

        /**
         * @brief Method for actual processing all
         * of added shortcuts.
         * @param input Pointer to input object.
         */
        void proceed(const HG::Core::Input* input);

        /**
         * @brief Method for adding shortcut to
         * processor.
         * @param shortcut Pointer to shortcut.
         */
        void addShortcut(Shortcut* shortcut);

        /**
         * @brief Method for removing shortcut
         * from processor.
         * @param shortcut Pointer to shortcut.
         */
        void removeShortcut(Shortcut* shortcut);

    private:

        std::vector<Shortcut*> m_shortcuts;

    };
}
