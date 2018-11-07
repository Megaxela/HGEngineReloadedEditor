#pragma once

// C++ STL
#include <vector>

// HG::Core
#include <HG/Core/Behaviour.hpp>

namespace HG::Core
{
    class GameObject;
    class Behaviour;
}

namespace HG::Editor::Widgets::Settings
{
    struct Common
    {
        enum class LastSelectedType
        {
            None,
            GameObject,
            Asset
        };

        LastSelectedType lastSelectedType = LastSelectedType::None;

        HG::Core::GameObject* selectedGameObject;

        std::vector<HG::Core::Behaviour*> behavioursCache;
        std::vector<HG::Core::GameObject*> gameobjectsCache;
        std::vector<HG::Core::Behaviour::Property> propertiesCache;
    };
}