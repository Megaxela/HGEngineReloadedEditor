#pragma once


namespace HG::Editor
{
    class Application;
}

namespace HG::Editor::Behaviours
{
    class GraphicsInterface;
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
         * @param graphicsInterface Pointer to parent application.
         */
        void setGraphicsInterface(HG::Editor::Behaviours::GraphicsInterface* graphicsInterface);

        /**
         * @brief Method for getting pointer to
         * parent application.
         * @return Pointer to parent application.
         */
        HG::Editor::Application* application() const;

        /**
         * @brief Method for getting pointer to parent
         * user interface behaviour.
         * @return Pointer to graphics interface.
         */
        HG::Editor::Behaviours::GraphicsInterface* graphicsInterface() const;

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

        /**
         * @brief Method, that performs widget initialization.
         * Has to be executed after application initialization.
         */
        void initialize();

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

        /**
         * @brief Method, that's called on `initialize` call.
         * By default do nothing.
         */
        virtual void onInitialization();

    private:

        HG::Editor::Behaviours::GraphicsInterface* m_graphicsInterface;
    };
}
