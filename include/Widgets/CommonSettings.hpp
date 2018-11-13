#pragma once

// C++ STL
#include <vector>
#include <memory>

// HG::Core
#include <HG/Core/Behaviour.hpp>

namespace HG::Core
{
    class GameObject;
    class Behaviour;
}

namespace HG::Rendering::Base
{
    class RenderBehaviour;
}

namespace HG::Editor::AssetSystem::Assets
{
    class AbstractAsset;

    using AssetPtr = std::shared_ptr<AbstractAsset>;
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
        HG::Editor::AssetSystem::Assets::AssetPtr selectedAsset;

        std::vector<HG::Core::Behaviour*> behavioursCache;
        std::vector<HG::Core::GameObject*> gameobjectsCache;
        std::vector<HG::Rendering::Base::RenderBehaviour*> renderBehavioursCache;
        std::vector<HG::Core::Behaviour::Property> propertiesCache;
    };
}