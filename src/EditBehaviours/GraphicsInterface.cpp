// Editor
#include <EditBehaviours/GraphicsInterface.hpp>
#include <Editor/Application.hpp>
#include <Fabrics/PropertyEditorsFabric.hpp>
#include <AbstractPropertyProcessor.hpp>

#include <Widgets/GameObjects.hpp>
#include <Widgets/Inspector.hpp>
#include <Widgets/Scene.hpp>
#include <Widgets/Assets.hpp>
#include <Widgets/Logging.hpp>

#include <Widgets/OpenPath.hpp>

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
    m_sceneWidget(new HG::Editor::Widgets::Scene()),
    m_loggingWidget(new HG::Editor::Widgets::Logging()),
    m_assetsWidget(new HG::Editor::Widgets::Assets(&m_commonSettings)),
    m_openPathWidget(new HG::Editor::Widgets::OpenPath()),
    m_renderOverride(new HG::Rendering::Base::RenderOverride)
{
    setupLogging();

    setupRenderOverride();
}

HG::Editor::Behaviours::GraphicsInterface::~GraphicsInterface()
{
    // todo: Delegate to method
    HG::Core::Logging::userLogger()->removeLogsListener(m_loggingWidget->logsListener());

    delete m_renderOverride->mainRenderTarget->colorTexture(0);
    delete m_renderOverride->mainRenderTarget;
    delete m_renderOverride;

    delete m_gameObjectsWidget;
    delete m_inspectorWidget;
    delete m_sceneWidget;
}

void HG::Editor::Behaviours::GraphicsInterface::onUpdate()
{
    // Setting render override object
    scene()->application()->renderer()->pipeline()->setRenderOverride(m_renderOverride);
    scene()->application()->renderer()->pipeline()->clear(HG::Utils::Color::fromRGB(25, 25, 25));

    m_loggingWidget->update();

    updateGameObjectsCache();

    prepareDockSpace();

    // Drawing widgets
    drawToolBar();

    m_gameObjectsWidget->draw();
    m_inspectorWidget->draw();
    m_sceneWidget->draw();
    m_loggingWidget->draw();
    m_assetsWidget->draw();

    // Required because of ImGui::Begin call in prepareDockSpace
    ImGui::End();

    m_openPathWidget->draw();
}

void HG::Editor::Behaviours::GraphicsInterface::onStart()
{
    m_gameObjectsWidget->setApplication(dynamic_cast<HG::Editor::Application *>(scene()->application()));
    m_inspectorWidget->setApplication(dynamic_cast<HG::Editor::Application *>(scene()->application()));
    m_sceneWidget->setApplication(dynamic_cast<HG::Editor::Application *>(scene()->application()));
    m_loggingWidget->setApplication(dynamic_cast<HG::Editor::Application *>(scene()->application()));
    m_assetsWidget->setApplication(dynamic_cast<HG::Editor::Application *>(scene()->application()));
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

void HG::Editor::Behaviours::GraphicsInterface::drawToolBar()
{
    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("New Project");
        if (ImGui::MenuItem("Open Project"))
        {
            actionOpenProject();
        }

        ImGui::MenuItem("Close Project");


        ImGui::Separator();

        if (ImGui::MenuItem("Exit"));

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("About HGEngine"));

        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
}

void HG::Editor::Behaviours::GraphicsInterface::actionOpenProject()
{

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
    auto texture = new HG::Rendering::Base::Texture(
        {200, 200},
        HG::Rendering::Base::Texture::Format::RGB
    );

    m_renderOverride->mainRenderTarget = new HG::Rendering::Base::RenderTarget({200, 200});
    m_renderOverride->mainRenderTarget->setColorTexture(texture, 0);

    m_sceneWidget->setRenderTarget(m_renderOverride->mainRenderTarget);
}

void HG::Editor::Behaviours::GraphicsInterface::prepareDockSpace()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus |
                    ImGuiWindowFlags_NoMove;

    ImGui::Begin("EditorDockSpace#", nullptr, window_flags);

    ImGui::PopStyleVar(2);

    // Enabling dockspace
    ImGui::DockSpace(ImGui::GetID("EditorDockSpace"));
}
