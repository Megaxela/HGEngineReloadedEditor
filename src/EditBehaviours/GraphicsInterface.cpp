// Editor
#include <EditBehaviours/GraphicsInterface.hpp>

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
    m_gameObjectsWidgetSettings(),
    m_inspectorWidgetSettings(),
    m_loggingWidgetSettings(),
    m_assetsWidgetSettings(),
    m_sceneWidgetSettings(),
    m_gameObjectsCache(),
    m_renderOverride(new HG::Rendering::Base::RenderOverride)
{
    setupLogging();

    setupRenderOverride();
}

HG::Editor::Behaviours::GraphicsInterface::~GraphicsInterface()
{
    // todo: Delegate to method
    HG::Core::Logging::userLogger()->removeLogsListener(m_loggingWidgetSettings.logsListener);

    delete m_renderOverride->mainRenderTarget->colorTexture(0);
    delete m_renderOverride->mainRenderTarget;
    delete m_renderOverride;
}

void HG::Editor::Behaviours::GraphicsInterface::onUpdate()
{
    // Setting render override object
    scene()->application()->renderer()->pipeline()->setRenderOverride(m_renderOverride);
    scene()->application()->renderer()->pipeline()->clear(HG::Utils::Color::fromRGB(25, 25, 25));

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

void HG::Editor::Behaviours::GraphicsInterface::updateRenderOverride()
{
    m_renderOverride->mainRenderTarget->setSize(m_sceneWidgetSettings.size);
    m_renderOverride->mainRenderTarget->colorTexture(0)->setSize(m_sceneWidgetSettings.size);
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
        switch (m_commonSettings.lastSelectedType)
        {
        case CommonSettings::LastSelectedType::GameObject:
            drawGameObjectInspectorBody();
            break;
        case CommonSettings::LastSelectedType::Asset:
            drawAssetInspectorBody();
            break;
        case CommonSettings::LastSelectedType::None:
            break;
        }
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
        m_sceneWidgetSettings.size = {
            ImGui::GetContentRegionAvail().x,
            ImGui::GetContentRegionAvail().y
        };

        updateRenderOverride();

        ImGui::Image(
            m_renderOverride->mainRenderTarget->colorTexture(0),
            ImVec2(m_sceneWidgetSettings.size.x, m_sceneWidgetSettings.size.y)
        );

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

void HG::Editor::Behaviours::GraphicsInterface::drawGameObjectInspectorBody()
{
    if (m_gameObjectsWidgetSettings.selected == nullptr)
    {
        return;
    }

    // Displaying gameobject header
    bool enabledCache = m_gameObjectsWidgetSettings.selected->isEnabled();
    if (ImGui::Checkbox("##InspectorEnabled", &enabledCache))
    {
        m_gameObjectsWidgetSettings.selected->setEnabled(enabledCache);
    }

    ImGui::SameLine();

    ImGui::Text("%s", m_gameObjectsWidgetSettings.selected->name().c_str());

    // todo: Don't invalidate cache if current gameobject was not changed
    //       or if behaviours amount was not changed.
    m_inspectorWidgetSettings.behavioursCache.clear();

    // Getting and iterating all behaviours
    m_gameObjectsWidgetSettings.selected->getBehaviours(m_inspectorWidgetSettings.behavioursCache);

    for (const auto& behaviour : m_inspectorWidgetSettings.behavioursCache)
    {
        m_inspectorWidgetSettings.propertiesCache.clear();

        // Getting and iterating all properties
        behaviour->getProperties(m_inspectorWidgetSettings.propertiesCache);

        for (const auto& property : m_inspectorWidgetSettings.propertiesCache)
        {

        }
    }
}

void HG::Editor::Behaviours::GraphicsInterface::drawAssetInspectorBody()
{

}

void HG::Editor::Behaviours::GraphicsInterface::setupRenderOverride()
{
    auto texture = new HG::Rendering::Base::Texture(
        {200, 200},
        HG::Rendering::Base::Texture::Format::RGB
    );

    m_renderOverride->mainRenderTarget = new HG::Rendering::Base::RenderTarget({200, 200});
    m_renderOverride->mainRenderTarget->setColorTexture(texture, 0);
}

void HG::Editor::Behaviours::GraphicsInterface::updateLogs()
{
    while (m_loggingWidgetSettings.logsListener->hasMessages())
    {
        m_loggingWidgetSettings.messagesBuffer.push_back(m_loggingWidgetSettings.logsListener->popMessage());
    }
}
