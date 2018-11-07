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
     * @brief Class, that describes assets widget.
     */
    class Assets : public AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit Assets(HG::Editor::Widgets::Settings::Common* settings);

    protected:
        void onDraw() override;

    private:

        HG::Editor::Widgets::Settings::Common* m_commonSettings;
    };
}