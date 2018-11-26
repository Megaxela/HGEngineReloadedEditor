// C++ STL
#include <algorithm>

// Editor
#include <Widgets/Assets.hpp>
#include <Editor/Application.hpp>
#include <Editor/ProjectController.hpp>
#include <AssetSystem/AssetsManager.hpp>
#include <AssetSystem/Assets/AbstractAsset.hpp>

// ImGui
#include <imgui.h>
#include <HG/Utils/StringTools.hpp>
#include <Widgets/CommonSettings.hpp>

HG::Editor::Widgets::Assets::Assets(HG::Editor::Widgets::Settings::Common *settings) :
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
    m_currentSorting(0)
{

}

void HG::Editor::Widgets::Assets::onDraw()
{
    auto flags = ImGuiWindowFlags_MenuBar;

    // todo: replace with HG::ID value
    if (ImGui::Begin("Assets", &m_opened, flags))
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
        auto flags = (asset->children().empty()                ? ImGuiTreeNodeFlags_Leaf     : 0U) |
                     (asset == m_commonSettings->selectedAsset ? ImGuiTreeNodeFlags_Selected : 0U) |
                                                                 ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                                                 ImGuiTreeNodeFlags_OpenOnArrow;

        expanded = ImGui::TreeNodeEx(asset.get(), flags, "%s", asset->name().c_str());

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
    ImGui::BeginMenuBar();

    // todo: replace with HG::ID value
    if (ImGui::BeginMenu("Sort"))
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
    // todo: replace with HG::ID value
    ImGui::BeginChild(static_cast<ImGuiID>(HG::Utils::StringTools::hash(__FUNCTION__)));

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
