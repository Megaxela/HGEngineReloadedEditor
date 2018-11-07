#pragma once

// Editor
#include <Widgets/AbstractWidget.hpp>

namespace HG::Editor::Widgets::Settings
{
    struct Common;
}

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes inspector editing widget.
     */
    class Inspector : public AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit Inspector(HG::Editor::Widgets::Settings::Common* settings);

    protected:

        void onDraw() override;

    private:

        /**
        * @brief Method for drawing and operating with
        * selected gameobject attributes.
        */
        void drawGameObjectBody();

        /**
         * @brief Method for drawing and operating with
         * selected asset.
         */
        void drawAssetBody();

        HG::Editor::Widgets::Settings::Common* m_commonSettings;
    };
}