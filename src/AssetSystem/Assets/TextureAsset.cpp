// Editor
#include <AssetSystem/Assets/TextureAsset.hpp>
#include <AssetSystem/AssetsManager.hpp>
#include <Editor/Application.hpp>

// HG::Core
#include <HG/Core/ResourceManager.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>
#include <HG/Utils/Surface.hpp>

HG::Editor::AssetSystem::Assets::TextureAsset::TextureAsset(std::filesystem::path path) :
    HG::Editor::AssetSystem::Assets::AbstractAsset(std::move(path), TextureAsset::AssetId),
    m_thumbnailBaseTexture(nullptr),
    m_thumbnailHandle(HG::Editor::ThumbnailsCache::InvalidHandle)
{

}

HG::Editor::ThumbnailsCache::Handle HG::Editor::AssetSystem::Assets::TextureAsset::icon() const
{
    return m_thumbnailHandle;
}

bool HG::Editor::AssetSystem::Assets::TextureAsset::onLoad()
{
    // Opening texture image
    auto image = assetsManager()
        ->application()
        ->resourceManager()
        ->load<HG::Utils::STBImageLoader>(path())
        .guaranteeGet(); // Due to async loading

    // Discards it if it was not opened
    if (image == nullptr)
    {
        return false;
    }

    // Creating texture for this image
    m_thumbnailBaseTexture =
        new (assetsManager()->application()->resourceCache())
        HG::Rendering::Base::Texture(image);

    // Calculating proper size.
    auto thumbnailSize = m_thumbnailBaseTexture->size();

    // Maximum size of thumbnail's side is 128
    const int maxSize = 128;

    double ratio = thumbnailSize.x / static_cast<double>(thumbnailSize.y);

    if (thumbnailSize.x > maxSize)
    {
        thumbnailSize.x = maxSize;
        thumbnailSize.y = static_cast<int>(maxSize / ratio);
    }
    else if (thumbnailSize.y > maxSize)
    {
        thumbnailSize.x = static_cast<int>(maxSize * ratio);
        thumbnailSize.y = maxSize;
    }

    // Posting this texture to thumbnails cache
    m_thumbnailHandle = assetsManager()
        ->application()
        ->thumbnailsCache()
        ->addThumbnail(m_thumbnailBaseTexture, thumbnailSize);
}

void HG::Editor::AssetSystem::Assets::TextureAsset::onPostLoad()
{
    delete m_thumbnailBaseTexture;
    m_thumbnailBaseTexture = nullptr;
}
