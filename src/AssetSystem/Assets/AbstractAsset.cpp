// Editor
#include <AssetSystem/Assets/AbstractAsset.hpp>

HG::Editor::AssetSystem::Assets::AbstractAsset::AbstractAsset(std::filesystem::path path, std::size_t type) :
    m_state(State::LoadingPending),
    m_path(std::move(path)),
    m_type(type)
{

}

HG::Editor::AssetSystem::Assets::AbstractAsset::~AbstractAsset()
{

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
