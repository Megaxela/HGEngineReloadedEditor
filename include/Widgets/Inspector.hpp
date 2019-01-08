#pragma once

// C++ STL
#include <cstdint>

// Editor
#include <Widgets/AbstractWidget.hpp>

namespace HG::Core
{
    class Transform;
}

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

        /**
         * @brief Method for drawing position editors.
         * @param gameObject Pointer to gameobject.
         */
        void drawTransformEdit(HG::Core::Transform* gameObject);

        /**
         * @brief Method for drawing "Add Behaviour" button.
         */
        void drawAddBehaviourButton();

        HG::Editor::Widgets::Settings::Common* m_commonSettings;
    };
}