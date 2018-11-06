#pragma once

// C++ STL
#include <memory>

// Editor
#include <Widgets/CommonSettings.hpp>

// HG::Core
#include <HG/Core/Behaviour.hpp>

// ALogger
#include <Loggers/AbstractLogger.hpp>

// Ringbuffer
#include <ringbuffer.hpp>

// GLM
#include <glm/glm.hpp>

namespace HG::Editor::Widgets
{
    class GameObjects;
}

namespace Logger
{
    class LogsListener;

    using LogsListenerPtr = std::shared_ptr<LogsListener>;
}

namespace HG::Core
{
    class Behaviour;
}

namespace HG::Rendering::Base
{
    class RenderTarget;
    struct RenderOverride;
}

namespace HG::Editor::Behaviours
{
    /**
     * @brief Behaviour class, that implements editor
     * graphical interface.
     */
    class GraphicsInterface : public HG::Core::Behaviour
    {
    public:

        /**
         * @brief Constructor.
         */
        GraphicsInterface();

        /**
         * @brief Destructor.
         */
        ~GraphicsInterface() override;

    protected:

        /**
         * @brief Method, that's called every frame and renders
         * GUI.
         */
        void onUpdate() override;

        void onStart() override;

    private:

        struct GameObjectsWidgetSettings
        {
            bool show = true;

            HG::Core::GameObject* selected = nullptr;
        };

        struct InspectorWidgetSettings
        {
            bool show = true;
            std::vector<HG::Core::Behaviour*> behavioursCache;
            std::vector<HG::Core::Behaviour::Property> propertiesCache;
        };

        struct LoggingWidgetSettings
        {
            bool show = true;

            Logger::LogsListenerPtr logsListener;

            ringbuffer<AbstractLogger::Message, 1000> messagesBuffer;
        };

        struct AssetsWidgetSettings
        {
            bool show = true;
        };

        struct SceneWidgetSettings
        {
            bool show = true;
            glm::ivec2 size;
        };

        /**
         * @brief Method for updating render override
         * object and override render target.
         */
        void updateRenderOverride();

        /**
         * @brief Method for updating internal
         * gameobjects cache if it's available.
         */
        void updateGameObjectsCache();

        /**
         * @brief Method for drawing toolbar.
         */
        void drawToolBar();

        /**
         * @brief Method for drawing and operating with
         * widget, that displays and manages gameobjects.
         */
        void drawGameObjectsWidget();

        /**
         * @brief Method for drawing and operating with
         * widget, that displays and manages selected entities
         * parameters. (GameObject's behaviours, Assets, etc.)
         */
        void drawInspectorWidget();

        /**
         * @brief Method for drawing and operating with
         * widget, that displays logs.
         */
        void drawLoggingWidget();

        /**
         * @brief Method for drawing and operating with
         * widget, that displays project's assets.
         */
        void drawAssetsWidget();

        /**
         * @brief Method for drawing and operating with
         * widget, that displays scene's editing widget.
         */
        void drawSceneWidget();

        // Actions
        void actionOpenProject();

        // GameObjectWidget methods

        /**
         * @brief Method for displaying gameobject and it's children.
         * This method is called recursively.
         * @param gameObject Pointer to gameobject to display.
         */
        void displayGameObject(HG::Core::GameObject* gameObject);

        // LoggingWidget methods

        /**
         * @brief Method for setting up user logging for
         * logging widget.
         */
        void setupLogging();

        // InspectorWidget methods

        /**
         * @brief Method for drawing and operating with
         * selected gameobject attributes.
         */
        void drawGameObjectInspectorBody();

        /**
         * @brief Method for drawing and operating with
         * selected asset.
         */
        void drawAssetInspectorBody();

        /**
         * @brief Method for setting up render override
         * system.
         */
        void setupRenderOverride();

        /**
         * @brief Method for transfering logs from listener
         * to internal logs ringbuffer.
         */
        void updateLogs();

        Widgets::Settings::Common m_commonSettings;

        HG::Editor::Widgets::GameObjects* m_gameObjectsWidget;

        GameObjectsWidgetSettings m_gameObjectsWidgetSettings;
        InspectorWidgetSettings m_inspectorWidgetSettings;
        LoggingWidgetSettings m_loggingWidgetSettings;
        AssetsWidgetSettings m_assetsWidgetSettings;
        SceneWidgetSettings m_sceneWidgetSettings;

        std::vector<HG::Core::GameObject*> m_gameObjectsCache;
        HG::Rendering::Base::RenderOverride* m_renderOverride;
    };
}


