#pragma once

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
    };
}