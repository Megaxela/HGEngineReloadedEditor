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
#include <Tools/ImGuiWidgets.hpp>

namespace HG::Editor
{
    class Application;
}

namespace HG::Editor::Widgets
{
    class AbstractWidget;
    class GameObjects;
    class Inspector;
    class Scene;
    class Assets;
    class Logging;
    class BenchmarkView;
    class TextureCacheView;

    class OpenPath;
    class InformationModal;
    class LineInputModal;
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
    class TransformBehaviour;

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

        /**
         * @brief Method for getting parent's editor application.
         * @return Pointer to editor application.
         */
        HG::Editor::Application* editorApplication() const;

        /**
         * @brief Method for getting pointer to
         * scene widget.
         * @return Pointer to scene widget.
         */
        HG::Editor::Widgets::Scene* sceneWidget() const;

        /**
         * @brief Method for getting pointer to line input modal
         * widget.
         * @return Pointer to line input modal widget.
         */
        HG::Editor::Widgets::LineInputModal* lineInputModal() const;

        /**
         * @brief Method for getting pointer to information modal
         * widget.
         * @return Pointer to information modal widget.
         */
        HG::Editor::Widgets::InformationModal* informationModal() const;

    protected:

        /**
         * @brief Method, that's called every frame and renders
         * GUI.
         */
        void onUpdate() override;

        /**
         * @brief Method, that's called once after initialization.
         */
        void onStart() override;

    private:

        /**
         * @brief Method for setting up
         * main menu.
         */
        void setupMainMenu();

        /**
         * @brief Method for updating internal
         * gameobjects cache if it's available.
         */
        void updateGameObjectsCache();

        // Actions
        void actionOpenProject();

        /**
         * @brief Method for handling gameobject
         * selection change and operating HG::Editor::Behaviours::TransformBehaviour
         */
        void handleGameObjectSelectionChange();

        /**
         * @brief Method for setting up user logging for
         * logging widget.
         */
        void setupLogging();

        /**
         * @brief Method for setting up render override
         * system.
         */
        void setupRenderOverride();

        /**
         * @brief Method for getting dockspace prepared.
         */
        void prepareDockSpace();

        Widgets::Settings::Common m_commonSettings;

        HG::Editor::Widgets::GameObjects* m_gameObjectsWidget;
        HG::Editor::Widgets::Inspector* m_inspectorWidget;
        HG::Editor::Widgets::Scene* m_sceneWidget;
        HG::Editor::Widgets::Logging* m_loggingWidget;
        HG::Editor::Widgets::Assets* m_assetsWidget;
        HG::Editor::Widgets::BenchmarkView* m_benchmarkWidget;
        HG::Editor::Widgets::TextureCacheView* m_textureCacheViewWidget;

        HG::Editor::Widgets::OpenPath* m_openPathWidget;
        HG::Editor::Widgets::InformationModal* m_informationModalWidget;
        HG::Editor::Widgets::LineInputModal* m_lineInputModalWidget;

        std::vector<HG::Editor::Widgets::AbstractWidget*> m_dockWidgets;
        std::vector<HG::Editor::Widgets::AbstractWidget*> m_commonWidgets;

        HG::Rendering::Base::RenderOverride* m_renderOverride;

        HG::Editor::Behaviours::TransformBehaviour* m_transformBehaviour;

        ImGui::Menu m_mainMenu;
    };
}


