#pragma once

// C++ STL
#include <unordered_set>

// HG::Core
#include <HG/Core/Input.hpp>

namespace HG::Editor
{
    struct Shortcut
    {
        using KeysSet = std::unordered_set<HG::Core::Input::Keyboard::Key>;
        using ModifiersSet = std::unordered_set<HG::Core::Input::Keyboard::Modifiers>;
        using Callback = std::function<void()>;

        KeysSet keys;
        ModifiersSet modifiers;

        Callback callback;
        bool fired = false;

        std::string generateName() const;
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
         * @brief Destructor.
         */
        ~ShortcutsProcessor();

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
        Shortcut* addShortcut(Shortcut::ModifiersSet modifiers, Shortcut::KeysSet shortcuts, Shortcut::Callback callback=nullptr);

        /**
         * @brief Method for removing shortcut
         * from processor. It's also disposes
         * shortcut.
         * @param shortcut Pointer to shortcut.
         */
        void removeShortcut(Shortcut* shortcut);

    private:

        std::vector<Shortcut*> m_shortcuts;

    };
}
