// C++ STL
#include <algorithm>

// Editor
#include <AssetSystem/Assets/AbstractAsset.hpp>

HG::Editor::AssetSystem::Assets::AbstractAsset::AbstractAsset(std::filesystem::path path, std::size_t type) :
    m_parent(nullptr),
    m_children(),
    m_state(State::LoadingPending),
    m_path(std::move(path)),
    m_type(type)
{

}

HG::Editor::AssetSystem::Assets::AbstractAsset::~AbstractAsset()
{
    for (auto& child : m_children)
    {
        delete child;
        child = nullptr;
    }

    m_children.clear();
}

std::string HG::Editor::AssetSystem::Assets::AbstractAsset::name() const
{
    return m_path.string();
}

std::filesystem::path HG::Editor::AssetSystem::Assets::AbstractAsset::path() const
{
    return m_path;
}

std::size_t HG::Editor::AssetSystem::Assets::AbstractAsset::type() const
{
    return m_type;
}

HG::Editor::AssetSystem::Assets::AbstractAsset::State HG::Editor::AssetSystem::Assets::AbstractAsset::state() const
{
    return m_state;
}

HG::Editor::AssetSystem::Assets::AbstractAsset *HG::Editor::AssetSystem::Assets::AbstractAsset::parent() const
{
    return m_parent;
}

std::vector<HG::Editor::AssetSystem::Assets::AbstractAsset *> &HG::Editor::AssetSystem::Assets::AbstractAsset::children()
{
    return m_children;
}

void HG::Editor::AssetSystem::Assets::AbstractAsset::setParent(HG::Editor::AssetSystem::Assets::AbstractAsset *parent)
{
    if (m_parent != nullptr)
    {
        m_parent->m_children.erase(
            std::find(
                m_parent->m_children.begin(),
                m_parent->m_children.end(),
                this
            )
        );
    }

    m_parent = parent;

    if (m_parent != nullptr)
    {
        m_parent->m_children.push_back(this);
    }
}

bool HG::Editor::AssetSystem::Assets::AbstractAsset::load() const
{
    return false;
}

bool HG::Editor::AssetSystem::Assets::AbstractAsset::onLoad()
{
    return false;
}

void HG::Editor::AssetSystem::Assets::AbstractAsset::onInspector()
{

}
