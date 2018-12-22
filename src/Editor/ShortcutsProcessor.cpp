// C++ STL
#include <algorithm>
#include <sstream>

// HG::Editor
#include <Editor/ShortcutsProcessor.hpp>
#include <Tools/EnumIterator.hpp>

HG::Editor::ShortcutsProcessor::ShortcutsProcessor() :
    m_shortcuts()
{

}

HG::Editor::ShortcutsProcessor::~ShortcutsProcessor()
{
    for (auto&& shortcut : m_shortcuts)
    {
        delete shortcut;
    }
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
            if (shortcut->keys.count(key) != input->keyboard()->isPushed(key))
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

HG::Editor::Shortcut* HG::Editor::ShortcutsProcessor::addShortcut(Shortcut::ModifiersSet modifiers,
                                                                  Shortcut::KeysSet keys,
                                                                  Shortcut::Callback callback)
{
    auto newShortcut = new Shortcut;

    newShortcut->callback = std::move(callback);
    newShortcut->modifiers = std::move(modifiers);
    newShortcut->keys = std::move(keys);

    m_shortcuts.push_back(newShortcut);

    return newShortcut;
}

void HG::Editor::ShortcutsProcessor::removeShortcut(HG::Editor::Shortcut* shortcut)
{
    m_shortcuts.erase(std::find(m_shortcuts.begin(), m_shortcuts.end(), shortcut));
    delete shortcut;
}

std::string HG::Editor::Shortcut::generateName() const
{
    // todo: if available - use reflection to translate enum to string
    static std::unordered_map<HG::Core::Input::Keyboard::Modifiers, std::string> modifierToString = {
          {HG::Core::Input::Keyboard::Modifiers::Shift, "Shift"}
        , {HG::Core::Input::Keyboard::Modifiers::Ctrl,  "Ctrl"}
        , {HG::Core::Input::Keyboard::Modifiers::Alt,   "Alt"}
    };

    static std::unordered_map<HG::Core::Input::Keyboard::Key, std::string> keyToString = {
          {HG::Core::Input::Keyboard::Key::ESC, "ESC"}
        , {HG::Core::Input::Keyboard::Key::F1, "F1"}
        , {HG::Core::Input::Keyboard::Key::F2, "F2"}
        , {HG::Core::Input::Keyboard::Key::F3, "F3"}
        , {HG::Core::Input::Keyboard::Key::F4, "F4"}
        , {HG::Core::Input::Keyboard::Key::F5, "F5"}
        , {HG::Core::Input::Keyboard::Key::F6, "F6"}
        , {HG::Core::Input::Keyboard::Key::F7, "F7"}
        , {HG::Core::Input::Keyboard::Key::F8, "F8"}
        , {HG::Core::Input::Keyboard::Key::F9, "F9"}
        , {HG::Core::Input::Keyboard::Key::F10, "F10"}
        , {HG::Core::Input::Keyboard::Key::F11, "F11"}
        , {HG::Core::Input::Keyboard::Key::F12, "F12"}
        , {HG::Core::Input::Keyboard::Key::F13, "F13"}
        , {HG::Core::Input::Keyboard::Key::F14, "F14"}
        , {HG::Core::Input::Keyboard::Key::F15, "F15"}
        , {HG::Core::Input::Keyboard::Key::F16, "F16"}
        , {HG::Core::Input::Keyboard::Key::F17, "F17"}
        , {HG::Core::Input::Keyboard::Key::F18, "F18"}
        , {HG::Core::Input::Keyboard::Key::F19, "F19"}
        , {HG::Core::Input::Keyboard::Key::F20, "F20"}
        , {HG::Core::Input::Keyboard::Key::F21, "F21"}
        , {HG::Core::Input::Keyboard::Key::F22, "F22"}
        , {HG::Core::Input::Keyboard::Key::F23, "F23"}
        , {HG::Core::Input::Keyboard::Key::F24, "F24"}
        , {HG::Core::Input::Keyboard::Key::F25, "F25"}
        , {HG::Core::Input::Keyboard::Key::Tilda, "~"}
        , {HG::Core::Input::Keyboard::Key::N1, "N1"}
        , {HG::Core::Input::Keyboard::Key::N2, "N2"}
        , {HG::Core::Input::Keyboard::Key::N3, "N3"}
        , {HG::Core::Input::Keyboard::Key::N4, "N4"}
        , {HG::Core::Input::Keyboard::Key::N5, "N5"}
        , {HG::Core::Input::Keyboard::Key::N6, "N6"}
        , {HG::Core::Input::Keyboard::Key::N7, "N7"}
        , {HG::Core::Input::Keyboard::Key::N8, "N8"}
        , {HG::Core::Input::Keyboard::Key::N9, "N9"}
        , {HG::Core::Input::Keyboard::Key::N0, "N0"}
        , {HG::Core::Input::Keyboard::Key::Minus, "-"}
        , {HG::Core::Input::Keyboard::Key::Equal, "="}
        , {HG::Core::Input::Keyboard::Key::Backspace, "Backspace"}
        , {HG::Core::Input::Keyboard::Key::Tab, "Tab"}
        , {HG::Core::Input::Keyboard::Key::Q, "Q"}
        , {HG::Core::Input::Keyboard::Key::W, "W"}
        , {HG::Core::Input::Keyboard::Key::E, "E"}
        , {HG::Core::Input::Keyboard::Key::R, "R"}
        , {HG::Core::Input::Keyboard::Key::T, "T"}
        , {HG::Core::Input::Keyboard::Key::Y, "Y"}
        , {HG::Core::Input::Keyboard::Key::U, "U"}
        , {HG::Core::Input::Keyboard::Key::I, "I"}
        , {HG::Core::Input::Keyboard::Key::O, "O"}
        , {HG::Core::Input::Keyboard::Key::P, "P"}
        , {HG::Core::Input::Keyboard::Key::SquareBracketOpen, "["}
        , {HG::Core::Input::Keyboard::Key::SquareBracketClose, "]"}
        , {HG::Core::Input::Keyboard::Key::Backslash, "\\"}
        , {HG::Core::Input::Keyboard::Key::CapsLock, "CapsLock"}
        , {HG::Core::Input::Keyboard::Key::A, "A"}
        , {HG::Core::Input::Keyboard::Key::S, "S"}
        , {HG::Core::Input::Keyboard::Key::D, "D"}
        , {HG::Core::Input::Keyboard::Key::F, "F"}
        , {HG::Core::Input::Keyboard::Key::G, "G"}
        , {HG::Core::Input::Keyboard::Key::H, "H"}
        , {HG::Core::Input::Keyboard::Key::J, "J"}
        , {HG::Core::Input::Keyboard::Key::K, "K"}
        , {HG::Core::Input::Keyboard::Key::L, "L"}
        , {HG::Core::Input::Keyboard::Key::Semicolon, "Semicolon"}
        , {HG::Core::Input::Keyboard::Key::SingleQuotation, "'"}
        , {HG::Core::Input::Keyboard::Key::Return, "Return"}
        , {HG::Core::Input::Keyboard::Key::LeftShift, "LShift"}
        , {HG::Core::Input::Keyboard::Key::Z, "Z"}
        , {HG::Core::Input::Keyboard::Key::X, "X"}
        , {HG::Core::Input::Keyboard::Key::C, "C"}
        , {HG::Core::Input::Keyboard::Key::V, "V"}
        , {HG::Core::Input::Keyboard::Key::B, "B"}
        , {HG::Core::Input::Keyboard::Key::N, "N"}
        , {HG::Core::Input::Keyboard::Key::M, "M"}
        , {HG::Core::Input::Keyboard::Key::Comma, ","}
        , {HG::Core::Input::Keyboard::Key::Dot, "."}
        , {HG::Core::Input::Keyboard::Key::Slash, "/"}
        , {HG::Core::Input::Keyboard::Key::RightShift, "RShift"}
        , {HG::Core::Input::Keyboard::Key::LeftCtrl, "LCtrl"}
        , {HG::Core::Input::Keyboard::Key::RightSuper, "RSuper"}
        , {HG::Core::Input::Keyboard::Key::LeftSuper, "LSuper"}
        , {HG::Core::Input::Keyboard::Key::LeftAlt, "LAlt"}
        , {HG::Core::Input::Keyboard::Key::Space, "Space"}
        , {HG::Core::Input::Keyboard::Key::RightAlt, "RAlt"}
        , {HG::Core::Input::Keyboard::Key::RightCtrl, "RCtrl"}
        , {HG::Core::Input::Keyboard::Key::ArrowLeft, "←"}
        , {HG::Core::Input::Keyboard::Key::ArrowUp, "↑"}
        , {HG::Core::Input::Keyboard::Key::ArrowDown, "↓"}
        , {HG::Core::Input::Keyboard::Key::ArrowRight, "→"}
        , {HG::Core::Input::Keyboard::Key::Insert, "Ins"}
        , {HG::Core::Input::Keyboard::Key::Delete, "Del"}
        , {HG::Core::Input::Keyboard::Key::PageUp, "PgUp"}
        , {HG::Core::Input::Keyboard::Key::PageDown, "PgDown"}
        , {HG::Core::Input::Keyboard::Key::Home, "Home"}
        , {HG::Core::Input::Keyboard::Key::End, "End"}
        , {HG::Core::Input::Keyboard::Key::NumLock, "NumLock"}
        , {HG::Core::Input::Keyboard::Key::PrintScreen, "PrintScreen"}
        , {HG::Core::Input::Keyboard::Key::Pause, "Pause"}
        , {HG::Core::Input::Keyboard::Key::Menu, "Menu"}
        , {HG::Core::Input::Keyboard::Key::Num0, "Num0"}
        , {HG::Core::Input::Keyboard::Key::Num1, "Num1"}
        , {HG::Core::Input::Keyboard::Key::Num2, "Num2"}
        , {HG::Core::Input::Keyboard::Key::Num3, "Num3"}
        , {HG::Core::Input::Keyboard::Key::Num4, "Num4"}
        , {HG::Core::Input::Keyboard::Key::Num5, "Num5"}
        , {HG::Core::Input::Keyboard::Key::Num6, "Num6"}
        , {HG::Core::Input::Keyboard::Key::Num7, "Num7"}
        , {HG::Core::Input::Keyboard::Key::Num8, "Num8"}
        , {HG::Core::Input::Keyboard::Key::Num9, "Num9"}
        , {HG::Core::Input::Keyboard::Key::NumDot, "NumDot"}
        , {HG::Core::Input::Keyboard::Key::NumDivide, "NumDivide"}
        , {HG::Core::Input::Keyboard::Key::NumMultiply, "NumMultiply"}
        , {HG::Core::Input::Keyboard::Key::NumSubstract, "NumSubstract"}
        , {HG::Core::Input::Keyboard::Key::NumAdd, "NumAdd"}
        , {HG::Core::Input::Keyboard::Key::NumReturn, "NumReturn"}
        , {HG::Core::Input::Keyboard::Key::NumEqual, "NumEqual"}
    };

    std::stringstream ss;

    for (auto modifier : modifiers)
    {
        auto iter = modifierToString.find(modifier);

        if (iter == modifierToString.end())
        {
            ss << '?';
        }
        else
        {
            ss << iter->second;
        }

        ss << '+';
    }

    bool first = true;

    for (auto key : keys)
    {
        if (!first)
        {
            ss << '+';
        }

        auto iter = keyToString.find(key);

        if (iter == keyToString.end())
        {
            ss << '?';
        }
        else
        {
            ss << iter->second;
        }

        first = false;
    }

    return ss.str();
}
