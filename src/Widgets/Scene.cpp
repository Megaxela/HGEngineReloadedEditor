// Editor
#include <Widgets/Scene.hpp>
#include <Tools/ImGuiWidgets.hpp>
#include <Editor/Application.hpp>
#include <Materials/ColorMaterial.hpp>

// HG::Core
#include <HG/Core/Scene.hpp>
#include <HG/Core/GameObject.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Texture.hpp>
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/RenderOverride.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>

// ImGui
#include <imgui.h>

// ALogger
#include <CurrentLogger.hpp>
#include <Widgets/CommonSettings.hpp>
#include <Tools/ImGuiIdentificators.hpp>

HG::Editor::Widgets::Scene::Scene(HG::Editor::Widgets::Settings::Common* common) :
    m_size({0, 0}),
    m_mainRenderTarget(nullptr),
    m_selectionOverride(new HG::Rendering::Base::RenderOverride()),
    m_materialOverride(nullptr),
    m_commonSettings(common)
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
    ImGui::IDGuard idGuard(HG::ID::Scene::Window);

    if (ImGui::Begin(HG::Names::Scene::Window, &m_opened))
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

                m_commonSettings->selectedGameObject = checkSelectedGameObject(pos);
                m_commonSettings->lastSelectedType = Settings::Common::LastSelectedType::GameObject;
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

HG::Core::GameObject *HG::Editor::Widgets::Scene::checkSelectedGameObject(ImVec2 pos)
{
    // Lazy initialization
    if (m_materialOverride == nullptr)
    {
        m_materialOverride = application()
                ->renderer()
                ->materialCollection()
                ->getMaterial<HG::Editor::Materials::ColorMaterial>();
        m_selectionOverride->material = m_materialOverride;
    }

    auto currentOverride = application()->renderer()->pipeline()->renderOverride();

    // Reusing already allocated render target (cause of render ordering)
    m_selectionOverride->mainRenderTarget = m_mainRenderTarget;

    // Enabling selection override
    application()->renderer()->pipeline()->setRenderOverride(m_selectionOverride);
    application()->renderer()->pipeline()->clear(HG::Utils::Color::Black);

    std::size_t diff = (256 * 256 * 256) / (m_commonSettings->gameobjectsCache.size() + 1);

    std::size_t colorCode = diff;

    m_colorCache.clear();
    // Rendering scene
    for (auto&& gameObject : m_commonSettings->gameobjectsCache)
    {
        m_commonSettings->renderBehavioursCache.clear();
        gameObject->getRenderingBehaviours(m_commonSettings->renderBehavioursCache);

        if (m_commonSettings->renderBehavioursCache.empty())
        {
            continue;
        }

        m_colorCache[colorCode] = gameObject;

        auto color = HG::Utils::Color::fromRGB(
                static_cast<uint8_t>((colorCode & 0xFF    )      ),
                static_cast<uint8_t>((colorCode & 0xFF00  ) >>  8),
                static_cast<uint8_t>((colorCode & 0xFF0000) >> 16)
        );

        m_materialOverride->setColor(color);

        Info() << "For GO " << gameObject->name() << " color " << color << " with code " << colorCode;

        for (auto&& renderBehaviour : m_commonSettings->renderBehavioursCache)
        {
            application()->renderer()->pipeline()->render(renderBehaviour);
        }
    }

    // Reverting previous override option
    application()->renderer()->pipeline()->setRenderOverride(currentOverride);

    // Getting data from rendertarget
    auto color = application()->renderer()->getTexturePixel(m_mainRenderTarget->colorTexture(0), {pos.x, pos.y});

    // Color to color code
    colorCode = color.red() | color.green() << 8 | color.blue() << 16;

    Info() << "Received color " << color << ", Received color code " << colorCode;

    // Searching in cache
    auto gameObjectIterator = m_colorCache.find(colorCode);

    if (gameObjectIterator == m_colorCache.end())
    {
        return nullptr;
    }

    return gameObjectIterator->second;
}
