#pragma once

namespace HG::Editor
{
    class Application;
}

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes abstract
     * ImGui widget.
     */
    class AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        AbstractWidget();

        /**
         * @brief Default virtual destructor.
         */
        virtual ~AbstractWidget() = default;

        /**
         * @brief Method for setting pointer to
         * parent application.
         * @param application Pointer to parent application.
         */
        void setApplication(HG::Editor::Application* application);

        /**
         * @brief Method for getting pointer to
         * parent application.
         * @return Pointer to parent application.
         */
        HG::Editor::Application* application() const;

        /**
         * @brief Method for setting widget as
         * opened or closed.
         */
        void setOpened(bool opened);

        /**
         * @brief Method for checking is widget opened or
         * closed.
         */
        bool isOpened() const;

        /**
         * @brief Method for drawing this widget.
         */
        void draw();

        /**
         * @brief Method, that has to be called before draw.
         */
        void update();

    protected:

        bool m_opened;

        /**
         * @brief Method, that's called on `draw` call.
         */
        virtual void onDraw() = 0;

        /**
         * @brief Method, that's called on `update` call.
         * By default do nothing.
         */
        virtual void onUpdate();

    private:

        HG::Editor::Application* m_application;
    };
}
