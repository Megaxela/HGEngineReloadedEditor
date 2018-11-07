// Editor
#include <Widgets/Scene.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Texture.hpp>

// ImGui
#include <imgui.h>

HG::Editor::Widgets::Scene::Scene() :
    m_size({0, 0}),
    m_mainRenderTarget(nullptr)
{

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
