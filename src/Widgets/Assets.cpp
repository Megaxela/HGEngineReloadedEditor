// C++ STL
#include <algorithm>

// Editor
#include <Widgets/Assets.hpp>
#include <Editor/Application.hpp>
#include <Editor/ProjectController.hpp>
#include <AssetSystem/AssetsManager.hpp>
#include <AssetSystem/Assets/AbstractAsset.hpp>
#include <Tools/ImGuiIdentificators.hpp>
#include <Tools/ImGuiWidgets.hpp>

// HG::Core
#include <HG/Core/ResourceManager.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>

// ImGui
#include <imgui.h>
#include <HG/Utils/StringTools.hpp>
#include <Widgets/CommonSettings.hpp>

HG::Editor::Widgets::Assets::Assets(HG::Editor::Widgets::Settings::Common *settings) :
    m_resourcesToFree(),
    m_commonSettings(settings),
    m_sortTypes({
        {"Name",
         [](const HG::Editor::AssetSystem::Assets::AssetPtr& l,
            const HG::Editor::AssetSystem::Assets::AssetPtr& r) -> bool
         {
             return l->name() < r->name();
         }},
        {"Type",
         [](const HG::Editor::AssetSystem::Assets::AssetPtr& l,
            const HG::Editor::AssetSystem::Assets::AssetPtr& r) -> bool
         {
             return l->type() < r->type();
         }}
    }),
    m_currentSorting(0),
    m_unloadedIcon (HG::Editor::ThumbnailsCache::InvalidHandle),
    m_loadingIcon  (HG::Editor::ThumbnailsCache::InvalidHandle),
    m_corruptedIcon(HG::Editor::ThumbnailsCache::InvalidHandle)
{

}

void HG::Editor::Widgets::Assets::onInitialization()
{
    auto unloadedIcon = new HG::Rendering::Base::Texture(
        application()
            ->resourceManager()
            ->load<HG::Utils::STBImageLoader>("images/unchecked.png")
            .guaranteeGet()
    );

    auto loadedIcon = new HG::Rendering::Base::Texture(
        application()
            ->resourceManager()
            ->load<HG::Utils::STBImageLoader>("images/checked.png")
            .guaranteeGet()
    );

    auto corruptedIcon = new HG::Rendering::Base::Texture(
        application()
            ->resourceManager()
            ->load<HG::Utils::STBImageLoader>("images/corrupted.png")
            .guaranteeGet()
    );

    m_resourcesToFree.push_back(unloadedIcon);
    m_resourcesToFree.push_back(loadedIcon);
    m_resourcesToFree.push_back(corruptedIcon);

    m_unloadedIcon  = application()->thumbnailsCache()->addThumbnail(unloadedIcon);
    m_loadingIcon   = application()->thumbnailsCache()->addThumbnail(loadedIcon);
    m_corruptedIcon = application()->thumbnailsCache()->addThumbnail(corruptedIcon);
}

void HG::Editor::Widgets::Assets::onPostInitialization()
{
    for (auto& resource : m_resourcesToFree)
    {
        delete resource;
    }

    m_resourcesToFree.clear();
}

void HG::Editor::Widgets::Assets::onDraw()
{
    auto flags = ImGuiWindowFlags_MenuBar;

    ImGui::IDGuard guard(HG::ID::Assets::Window);

    if (ImGui::Begin(HG::Names::Assets::Window, &m_opened, flags))
    {
        drawToolbar();

        drawItems();
    }
    ImGui::End();
}

void HG::Editor::Widgets::Assets::drawAsset(const HG::Editor::AssetSystem::Assets::AssetPtr& asset, bool isRoot)
{
    auto expanded = true;

    if (!isRoot)
    {
        auto flags = (asset->children().empty()                ? ImGuiTreeNodeFlags_Leaf     : 0) |
                     (asset == m_commonSettings->selectedAsset ? ImGuiTreeNodeFlags_Selected : 0U) |
                                                                 ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                                                 ImGuiTreeNodeFlags_OpenOnArrow;

        // Getting icon
        HG::Rendering::Base::Texture* thumbnailsTexture = nullptr;
        ImVec2 uvTL;
        ImVec2 uvBR;

        auto icon = m_loadingIcon;

        if (application()->thumbnailsCache()->isAvailable(icon))
        {
            thumbnailsTexture = application()->thumbnailsCache()->texture();
            auto tlbr = application()->thumbnailsCache()->thumbnailTLBR(icon);

            uvTL = ImGui::fromGLM(application()->thumbnailsCache()->pixelsToUV(tlbr.tl));
            uvBR = ImGui::fromGLM(application()->thumbnailsCache()->pixelsToUV(tlbr.br));
        }

        expanded = ImGui::IconTreeNodeEx(asset.get(), flags, thumbnailsTexture, uvTL, uvBR, "%s", asset->name().c_str());

        if (ImGui::BeginDragDropSource())
        {
            auto stringType = std::to_string(asset->type());

            ImGui::SetDragDropPayload(stringType.c_str(), &asset, sizeof(asset));

            ImGui::Text("%s", asset->name().c_str());

            ImGui::EndDragDropSource();
        }

        if (ImGui::IsItemClicked(0))
        {
            m_commonSettings->selectedAsset = asset;
            m_commonSettings->lastSelectedType = HG::Editor::Widgets::Settings::Common::LastSelectedType::Asset;
        }
    }

    if (expanded)
    {
        for (auto&& child : asset->children())
        {
            drawAsset(child, false);
        }

        if (!isRoot)
        {
            ImGui::TreePop();
        }
    }
}

void HG::Editor::Widgets::Assets::drawToolbar()
{
    ImGui::IDGuard guard(HG::ID::Assets::Sort);

    ImGui::BeginMenuBar();

    if (ImGui::BeginMenu(HG::Names::Assets::Sort))
    {
        for (std::size_t i = 0; i < m_sortTypes.size(); ++i)
        {
            if (ImGui::MenuItem(m_sortTypes[i].name.c_str(), nullptr, i == m_currentSorting))
            {
                m_currentSorting = i;

                performSorting();
            }
        }

        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
}

void HG::Editor::Widgets::Assets::drawItems()
{
    ImGui::BeginChild(HG::ID::Assets::ItemsChild);

    auto rootAsset = application()->projectController()->assetManager()->rootAsset();

    if (rootAsset)
    {
        drawAsset(rootAsset, true);
    }

    ImGui::EndChild();
}

void HG::Editor::Widgets::Assets::performSorting()
{
    auto root = application()->projectController()->assetManager()->rootAsset();

    if (root == nullptr)
    {
        return;
    }

    sortAssetChildren(root);
}

void HG::Editor::Widgets::Assets::sortAssetChildren(HG::Editor::AssetSystem::Assets::AssetPtr asset)
{
    std::sort(
        asset->children().begin(),
        asset->children().end(),
        m_sortTypes[m_currentSorting].comparator
    );

    for (auto&& child : asset->children())
    {
        sortAssetChildren(child);
    }
}
