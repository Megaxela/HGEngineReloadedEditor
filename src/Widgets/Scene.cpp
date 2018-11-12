// Editor
#include <Widgets/Scene.hpp>
#include <Tools/ImGuiWidgets.hpp>
#include <Editor/Application.hpp>

// HG::Core
#include <HG/Core/Scene.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Texture.hpp>
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/RenderOverride.hpp>

// ImGui
#include <imgui.h>

// ALogger
#include <CurrentLogger.hpp>

HG::Editor::Widgets::Scene::Scene() :
    m_size({0, 0}),
    m_mainRenderTarget(nullptr),
    m_selectionOverride(new HG::Rendering::Base::RenderOverride())
{

}

HG::Editor::Widgets::Scene::~Scene()
{
    delete m_selectionOverride;
}

void HG::Editor::Widgets::Scene::setRenderTarget(HG::Rendering::Base::RenderTarget *target)
{
    m_mainRenderTarget = target;
}

void HG::Editor::Widgets::Scene::onDraw()
{
    if (ImGui::Begin("Scene", &m_opened))
    {
        m_size = {
            ImGui::GetContentRegionAvail().x,
            ImGui::GetContentRegionAvail().y
        };

        updateRenderOverride();

        if (m_mainRenderTarget &&
            m_mainRenderTarget->colorTexture(0))
        {
            ImGui::Image(
                m_mainRenderTarget->colorTexture(0),
                ImVec2(m_size.x, m_size.y)
            );

            if (ImGui::IsItemClicked(0))
            {
                auto pos = ImGui::GetMousePos() - ImGui::GetItemRectMin();

                // todo: Trigger item locating with re rendering.
                auto selectedGameObject = checkSelectedGameObject();
            }
        }
    }
    ImGui::End();
}

void HG::Editor::Widgets::Scene::updateRenderOverride()
{
    if (m_mainRenderTarget->size() != m_size)
    {
        m_mainRenderTarget->setSize(m_size);
        m_mainRenderTarget->colorTexture(0)->setSize(m_size);
    }
}

HG::Core::GameObject *HG::Editor::Widgets::Scene::checkSelectedGameObject()
{
    auto currentOverride = application()->renderer()->pipeline()->renderOverride();

    // Reusing already allocated render target (cause of render ordering)
    m_selectionOverride->mainRenderTarget = m_mainRenderTarget;

    // Enabling selection override
    application()->renderer()->pipeline()->setRenderOverride(m_selectionOverride);

    // Rendering scene
    application()->scene()->render(application()->renderer());

    // todo: Add result processing

    // Reverting previous override option
    application()->renderer()->pipeline()->setRenderOverride(currentOverride);

    return nullptr;
}
