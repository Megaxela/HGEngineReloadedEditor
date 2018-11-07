#pragma once

// Editor
#include <Widgets/AbstractWidget.hpp>

namespace HG::Core
{
    class GameObject;
}

namespace HG::Editor::Widgets::Settings
{
    struct Common;
}

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes GameObjects widget.
     */
    class GameObjects : public AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit GameObjects(HG::Editor::Widgets::Settings::Common* settings);

    protected:

        void onDraw() override;

        /**
         * @brief Recursive method, that performs
         * gameobject's tree drawing.
         * @param go Pointer to gameobject to draw.
         */
        void displayGameObject(HG::Core::GameObject* go);

    private:

        HG::Editor::Widgets::Settings::Common* m_commonSettings;
    };
}