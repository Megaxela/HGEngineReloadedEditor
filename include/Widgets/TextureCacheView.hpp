#pragma once

// Editor
#include <Widgets/AbstractWidget.hpp>

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes widget for displaying all
     * active textures.
     */
    class TextureCacheView : public AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit TextureCacheView();

    protected:

        void onDraw() override;

    private:



    };
}
