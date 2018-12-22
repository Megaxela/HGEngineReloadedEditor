// C++ STL
#include <algorithm>

// HG::Editor
#include <Editor/ShortcutsProcessor.hpp>
#include <Tools/EnumIterator.hpp>

HG::Editor::ShortcutsProcessor::ShortcutsProcessor() :
    m_shortcuts()
{

}

void HG::Editor::ShortcutsProcessor::proceed(const HG::Core::Input* input)
{
    // todo: Optimize, due to mostly unused shit
    for (auto* shortcut : m_shortcuts)
    {
        shortcut->fired = false;
        bool failed = false;

        for (auto modifier : EnumIterator<HG::Core::Input::Keyboard::Modifiers>())
        {
            if (shortcut->modifiers.count(modifier) != input->keyboard()->isModifierPressed(modifier))
            {
                failed = true;
                break;
            }
        }

        if (failed)
        {
            continue;
        }

        for (auto key : EnumIterator<HG::Core::Input::Keyboard::Key>())
        {
            if (shortcut->keys.count(key) != input->keyboard()->isPressed(key))
            {
                failed = true;
                break;
            }
        }

        if (failed)
        {
            continue;
        }

        shortcut->fired = true;
        if (shortcut->callback)
        {
            shortcut->callback();
        }
    }
}

void HG::Editor::ShortcutsProcessor::addShortcut(HG::Editor::ShortcutsProcessor::Shortcut* shortcut)
{
    m_shortcuts.push_back(shortcut);
}

void HG::Editor::ShortcutsProcessor::removeShortcut(HG::Editor::ShortcutsProcessor::Shortcut* shortcut)
{
    m_shortcuts.erase(std::find(m_shortcuts.begin(), m_shortcuts.end(), shortcut));
}
