#pragma once

// Editor
#include <Widgets/AbstractWidget.hpp>

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
        GameObjects();

    protected:

        void onDraw() override;

    };
}