// Editor
#include <EditBehaviours/GraphicsInterface.hpp>

// HG::Core
#include <Scene.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Logging.hpp>

// ImGui
#include <imgui.h>

// ALogger
#include <CurrentLogger.hpp>
#include <LogsListener.hpp>

HG::Editor::Behaviours::GraphicsInterface::GraphicsInterface() :
    m_commonSettings(),
    m_gameObjectsWidgetSettings(),
    m_inspectorWidgetSettings(),
    m_loggingWidgetSettings(),
    m_assetsWidgetSettings(),
    m_sceneWidgetSettings(),
    m_gameObjectsCache()
{
    setupLogging();
}

HG::Editor::Behaviours::GraphicsInterface::~GraphicsInterface()
{
    // todo: Delegate to method
    HG::Core::Logging::userLogger()->removeLogsListener(m_loggingWidgetSettings.logsListener);
}

void HG::Editor::Behaviours::GraphicsInterface::onUpdate()
{
    updateLogs();
    updateGameObjectsCache();

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



    // Drawing widgets
    drawGameObjectsWidget();
    drawInspectorWidget();
    drawLoggingWidget();
    drawAssetsWidget();
    drawSceneWidget();

    ImGui::End();
}

void HG::Editor::Behaviours::GraphicsInterface::onStart()
{

}

void HG::Editor::Behaviours::GraphicsInterface::updateGameObjectsCache()
{
    m_gameObjectsCache.clear();

    if (scene() == nullptr)
    {
        return;
    }

    scene()->getGameObjects(m_gameObjectsCache);
}

void HG::Editor::Behaviours::GraphicsInterface::drawGameObjectsWidget()
{
    if (ImGui::Begin("GameObjects", &m_gameObjectsWidgetSettings.show))
    {
        // Displaying root gameobjects
        // (Root gameobjects does not have parent)
        for (auto&& gameObject : m_gameObjectsCache)
        {
            if (gameObject->transform()->parent() != nullptr)
            {
                continue;
            }

            displayGameObject(gameObject);
        }
    }
    ImGui::End();
}

void HG::Editor::Behaviours::GraphicsInterface::drawInspectorWidget()
{
    if (ImGui::Begin("Inspector", &m_inspectorWidgetSettings.show))
    {

    }
    ImGui::End();
}

void HG::Editor::Behaviours::GraphicsInterface::drawLoggingWidget()
{
    if (ImGui::Begin("Logs", &m_loggingWidgetSettings.show))
    {
        for (auto&& message : m_loggingWidgetSettings.messagesBuffer)
        {
            ImGui::Selectable("%s", message.message.c_str());
        }
    }
    ImGui::End();
}

void HG::Editor::Behaviours::GraphicsInterface::drawAssetsWidget()
{
    if (ImGui::Begin("Assets", &m_assetsWidgetSettings.show))
    {

    }
    ImGui::End();
}

void HG::Editor::Behaviours::GraphicsInterface::drawSceneWidget()
{
    if (ImGui::Begin("Scene", &m_sceneWidgetSettings.show))
    {

    }
    ImGui::End();
}

void HG::Editor::Behaviours::GraphicsInterface::displayGameObject(HG::Core::GameObject *gameObject)
{
    auto hasChildren = !gameObject->transform()->children().empty();

    auto nodeFlags =
        (hasChildren ? 0U : ImGuiTreeNodeFlags_Leaf) |
        ImGuiTreeNodeFlags_OpenOnArrow |
        (gameObject == m_gameObjectsWidgetSettings.selected ? ImGuiTreeNodeFlags_Selected : 0U);

    bool opened = ImGui::TreeNodeEx(gameObject, nodeFlags, "%s", gameObject->name().c_str());

    // If LMB pressed on this item
    if (ImGui::IsItemClicked(0))
    {
        m_commonSettings.lastSelectedType = CommonSettings::LastSelectedType::GameObject;
        m_gameObjectsWidgetSettings.selected = gameObject;
    }

    // If this gameobject opened - show
    // it's children.
    if (opened)
    {
        for (auto&& childTransform : gameObject->transform()->children())
        {
            displayGameObject(childTransform->gameObject());
        }

        ImGui::TreePop();
    }
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
    m_loggingWidgetSettings.logsListener = std::make_shared<UserLogsListener>();

    HG::Core::Logging::userLogger()->addLogsListener(m_loggingWidgetSettings.logsListener);
}

void HG::Editor::Behaviours::GraphicsInterface::updateLogs()
{
    while (m_loggingWidgetSettings.logsListener->hasMessages())
    {
        m_loggingWidgetSettings.messagesBuffer.push_back(m_loggingWidgetSettings.logsListener->popMessage());
    }
}
