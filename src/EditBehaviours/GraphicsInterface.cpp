// Editor
#include <EditBehaviours/GraphicsInterface.hpp>
#include <EditBehaviours/TransformBehaviour.hpp>
#include <Editor/Application.hpp>
#include <Fabrics/PropertyEditorsFabric.hpp>
#include <AbstractPropertyProcessor.hpp>
#include <Editor/ProjectController.hpp>

#include <Widgets/GameObjects.hpp>
#include <Widgets/Inspector.hpp>
#include <Widgets/Scene.hpp>
#include <Widgets/Assets.hpp>
#include <Widgets/Logging.hpp>

#include <Widgets/OpenPath.hpp>
#include <Widgets/InformationModal.hpp>
#include <Widgets/BenchmarkView.hpp>
#include <Widgets/TextureCacheView.hpp>
#include <Widgets/LineInputModal.hpp>

// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Transform.hpp>
#include <HG/Core/Logging.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Texture.hpp>
#include <HG/Rendering/Base/RenderOverride.hpp>

// ImGui
#include <imgui.h>

// ALogger
#include <CurrentLogger.hpp>
#include <LogsListener.hpp>

HG::Editor::Behaviours::GraphicsInterface::GraphicsInterface() :
    m_commonSettings(),
    m_gameObjectsWidget(new HG::Editor::Widgets::GameObjects(&m_commonSettings)),
    m_inspectorWidget(new HG::Editor::Widgets::Inspector(&m_commonSettings)),
    m_sceneWidget(new HG::Editor::Widgets::Scene(&m_commonSettings)),
    m_loggingWidget(new HG::Editor::Widgets::Logging()),
    m_assetsWidget(new HG::Editor::Widgets::Assets(&m_commonSettings)),
    m_benchmarkWidget(new HG::Editor::Widgets::BenchmarkView()),
    m_textureCacheViewWidget(new HG::Editor::Widgets::TextureCacheView()),
    m_openPathWidget(new HG::Editor::Widgets::OpenPath()),
    m_informationModalWidget(new HG::Editor::Widgets::InformationModal()),
    m_lineInputModalWidget(new HG::Editor::Widgets::LineInputModal()),
    m_dockWidgets(),
    m_commonWidgets(),
    m_renderOverride(new HG::Rendering::Base::RenderOverride),
    m_transformBehaviour(new HG::Editor::Behaviours::TransformBehaviour),
    m_mainMenu()
{
    m_dockWidgets.push_back(m_gameObjectsWidget);
    m_dockWidgets.push_back(m_inspectorWidget);
    m_dockWidgets.push_back(m_sceneWidget);
    m_dockWidgets.push_back(m_loggingWidget);
    m_dockWidgets.push_back(m_assetsWidget);
    m_dockWidgets.push_back(m_benchmarkWidget);
    m_dockWidgets.push_back(m_textureCacheViewWidget);
    m_commonWidgets.push_back(m_openPathWidget);
    m_commonWidgets.push_back(m_informationModalWidget);
    m_commonWidgets.push_back(m_lineInputModalWidget);
}

HG::Editor::Behaviours::GraphicsInterface::~GraphicsInterface()
{
    // todo: Delegate to method
    HG::Core::Logging::userLogger()->removeLogsListener(m_loggingWidget->logsListener());

    delete m_renderOverride->mainRenderTarget->colorTexture(0);
    delete m_renderOverride->mainRenderTarget;
    delete m_renderOverride;

    for (auto& widget : m_dockWidgets)
    {
        delete widget;
    }

    for (auto& widget : m_commonWidgets)
    {
        delete widget;
    }
}

HG::Editor::Widgets::Scene* HG::Editor::Behaviours::GraphicsInterface::sceneWidget() const
{
    return m_sceneWidget;
}

HG::Editor::Widgets::LineInputModal* HG::Editor::Behaviours::GraphicsInterface::lineInputModal() const
{
    return m_lineInputModalWidget;
}

void HG::Editor::Behaviours::GraphicsInterface::onUpdate()
{
    handleGameObjectSelectionChange();

    // Setting render override object
    scene()->application()->renderer()->pipeline()->setRenderOverride(m_renderOverride);
    scene()->application()->renderer()->pipeline()->clear(HG::Utils::Color::fromRGB(25, 25, 25));

    for (auto& widget : m_dockWidgets)
    {
        widget->update();
    }

    for (auto& widget : m_commonWidgets)
    {
        widget->update();
    }

    updateGameObjectsCache();

    prepareDockSpace();

    // Drawing widgets
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::MainMenuRenderer().render(m_mainMenu);

    // Drawing toolbar
    if (ImGui::BeginToolBar())
    {
        ImGui::Button("Move");
        ImGui::Button("Rotate");
        ImGui::Button("Scale");

        ImGui::EndToolBar();
    }
//    ImGui::PopStyleVar(1);

    for (auto& widget : m_dockWidgets)
    {
        widget->draw();
    }

    // Required because of ImGui::Begin call in prepareDockSpace
    ImGui::End();

    for (auto& widget : m_commonWidgets)
    {
        widget->draw();
    }
}

void HG::Editor::Behaviours::GraphicsInterface::onStart()
{
    gameObject()->addBehaviour(m_transformBehaviour);

    setupMainMenu();

    setupLogging();

    setupRenderOverride();

    // Setting application to widgets.
    for (auto& widget : m_dockWidgets)
    {
        widget->setGraphicsInterface(this);
    }

    for (auto& widget : m_commonWidgets)
    {
        widget->setGraphicsInterface(this);
    }

    // Initializing widgets
    for (auto& widget : m_dockWidgets)
    {
        widget->initialize();
    }

    for (auto& widget : m_commonWidgets)
    {
        widget->initialize();
    }
}

void HG::Editor::Behaviours::GraphicsInterface::setupMainMenu()
{
    auto shortcuts = editorApplication()->shortcutsProcessor();

    {
        auto menu = m_mainMenu.addMenu("File", "File");
        menu->addItem(
            "New Project",
            "New Project",
            shortcuts->addShortcut({
                HG::Core::Input::Keyboard::Modifiers::Ctrl
            }, {
                HG::Core::Input::Keyboard::Key::N
            }));

        menu->addItem(
            "Open Project",
            "OpenProject",
            shortcuts->addShortcut({
                HG::Core::Input::Keyboard::Modifiers::Ctrl
            }, {
                HG::Core::Input::Keyboard::Key::O
            }))->setCallback( [this]() { actionOpenProject(); } );

        menu->addItem("Close Project", "Close Project");
        menu->addSeparator();
        menu->addItem(
            "Exit",
            "Exit",
            shortcuts->addShortcut({
                HG::Core::Input::Keyboard::Modifiers::Ctrl,
            }, {
                HG::Core::Input::Keyboard::Key::Q
            }))->setCallback( [this]() { editorApplication()->stop(); } );
    }

    {
        auto menu = m_mainMenu.addMenu("Help", "Help");
        menu->addItem("About HGEngine", "About HGEngine");
    }
}

void HG::Editor::Behaviours::GraphicsInterface::updateGameObjectsCache()
{
    m_commonSettings.gameobjectsCache.clear();

    if (scene() == nullptr)
    {
        return;
    }

    scene()->getGameObjects(m_commonSettings.gameobjectsCache);
}

HG::Editor::Application* HG::Editor::Behaviours::GraphicsInterface::editorApplication() const
{
    return static_cast<HG::Editor::Application*>(scene()->application());
}

void HG::Editor::Behaviours::GraphicsInterface::actionOpenProject()
{
    auto project = editorApplication()->projectController();

    m_openPathWidget->settings().mode = HG::Editor::Widgets::OpenPath::Settings::Mode::Directory;
    m_openPathWidget->setOkCallback(
        [project, this](const std::filesystem::path& data)
        {
            try
            {
                project->load(data);
            }
            catch (const std::invalid_argument& argument)
            {
                m_informationModalWidget->error(std::string("Can't open project: ") + argument.what());
                ErrorF() << "Can't open project: " << argument.what();
            }
        }
    );

    m_openPathWidget->setOpened(true);
}

/**
 * @brief Logs listener for user logs.
 */
class UserLogsListener : public Logger::LogsListener
{
public:

    AbstractLogger::Message popMessage() override
    {
        auto msg = m_messages.front();
        m_messages.pop();

        return msg;
    }

    bool hasMessages() const override
    {
        return !m_messages.empty();
    }

    void newMessage(const AbstractLogger::Message &m) override
    {
        m_messages.push(m);
    }

private:

    std::queue<AbstractLogger::Message> m_messages;
};

void HG::Editor::Behaviours::GraphicsInterface::setupLogging()
{
    auto logsListener = std::make_shared<UserLogsListener>();

    HG::Core::Logging::userLogger()->addLogsListener(logsListener);

    m_loggingWidget->setLogsListener(logsListener);
}

void HG::Editor::Behaviours::GraphicsInterface::setupRenderOverride()
{
    auto texture = new (scene()->application()->resourceCache()) HG::Rendering::Base::Texture(
        {200, 200},
        HG::Rendering::Base::Texture::Format::RGB
    );

    m_renderOverride->mainRenderTarget = new (scene()->application()->resourceCache()) HG::Rendering::Base::RenderTarget({200, 200});
    m_renderOverride->mainRenderTarget->setColorTexture(texture, 0);

    m_sceneWidget->setRenderTarget(m_renderOverride->mainRenderTarget);
}

void HG::Editor::Behaviours::GraphicsInterface::prepareDockSpace()
{
    ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoMove;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});

    ImGui::Begin("EditorDockSpace#", nullptr, window_flags);

    ImGui::PopStyleVar(3);

    // Enabling dockspace
    ImGui::DockSpaceEx(ImGui::GetID("EditorDockSpace"), {0, 0}, ImGuiDockNodeFlagsEx_ToolBar);
}

void HG::Editor::Behaviours::GraphicsInterface::handleGameObjectSelectionChange()
{
    m_transformBehaviour->configuration()->sceneWidgetRect = HG::Utils::Rect(
        m_sceneWidget->getTopLeftPosition().x,
        m_sceneWidget->getTopLeftPosition().y,
        m_renderOverride->mainRenderTarget->size().x,
        m_renderOverride->mainRenderTarget->size().y
    );

    m_transformBehaviour->setEnabled(m_sceneWidget->isShown());

    m_transformBehaviour->setSelectedGameObject(m_commonSettings.selectedGameObject);
}
