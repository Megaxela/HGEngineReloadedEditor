// Editor
#include <Tools/GlobalThumbnails.hpp>
#include <Editor/Application.hpp>

// HG::Core
#include <HG/Core/ResourceManager.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>

HG::Editor::GlobalThumbnails::GlobalThumbnails(HG::Editor::Application* application) :
    m_handles(),
    m_textures(),
    m_application(application)
{

}

HG::Editor::GlobalThumbnails::~GlobalThumbnails()
{
    auto cache = application()->thumbnailsCache();

    for (auto& [code, handle] : m_handles)
    {
        cache->removeThumbnail(handle);
    }

    finishLoading();
}

HG::Editor::Application* HG::Editor::GlobalThumbnails::application() const
{
    return m_application;
}

HG::Editor::ThumbnailsCache::Handle HG::Editor::GlobalThumbnails::getHandle(HG::Editor::GlobalThumbnails::Thumbs thumb)
{
    auto iter = m_handles.find(thumb);

    if (iter == m_handles.end())
    {
        return HG::Editor::ThumbnailsCache::InvalidHandle;
    }

    return iter->second;
}

void HG::Editor::GlobalThumbnails::startLoading()
{
    loadInternal(Thumbs::DirectoryIcon,      "images/folder.png");
    loadInternal(Thumbs::FileIcon,           "images/text.png");
    loadInternal(Thumbs::UnloadedAssetIcon,  "images/unchecked.png");
    loadInternal(Thumbs::LoadingAssetIcon,   "images/checked.png");
    loadInternal(Thumbs::CorruptedAssetIcon, "images/corrupted.png");
    loadInternal(Thumbs::ErrorMark,          "images/error.png");
}

void HG::Editor::GlobalThumbnails::finishLoading()
{
    for (auto& texture : m_textures)
    {
        delete texture;
    }

    m_textures.clear();
}

void HG::Editor::GlobalThumbnails::loadInternal(Thumbs thumb, std::filesystem::path path)
{
    auto texture = new (application()->resourceCache()) HG::Rendering::Base::Texture(
        application()
            ->resourceManager()
            ->load<HG::Utils::STBImageLoader>(path)
    );

    m_textures.push_back(texture);
    m_handles.emplace(thumb, application()->thumbnailsCache()->addThumbnail(texture));
}
