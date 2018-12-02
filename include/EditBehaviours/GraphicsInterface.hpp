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

namespace HG::Editor::Widgets
{
    class AbstractWidget;
    class GameObjects;
    class Inspector;
    class Scene;
    class Assets;
    class Logging;
    class BenchmarkView;

    class OpenPath;
    class InformationModal;
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

        HG::Editor::Widgets::OpenPath* m_openPathWidget;
        HG::Editor::Widgets::InformationModal* m_informationModalWidget;

        std::vector<HG::Editor::Widgets::AbstractWidget*> m_dockWidgets;
        std::vector<HG::Editor::Widgets::AbstractWidget*> m_commonWidgets;

        HG::Rendering::Base::RenderOverride* m_renderOverride;

        ImGui::Menu m_mainMenu;
    };
}


