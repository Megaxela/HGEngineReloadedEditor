#pragma once

// C++ STL
#include <unordered_map>
#include <filesystem>

// Editor
#include <Tools/ThumbnailsCache.hpp>

namespace HG::Editor
{
    class Application;

    /**
     * @brief Class, that handles global thumbnails.
     */
    class GlobalThumbnails
    {
    public:

        /**
         * @brief Enum with global thumbnails identificators.
         *
         */
        enum class Thumbs
        {
            DirectoryIcon,
            FileIcon,
            UnloadedAssetIcon,
            LoadingAssetIcon,
            CorruptedAssetIcon,
            ErrorMark,
            WarningMark,
            InfoMark
        };

        /**
         * @brief Constructor.
         * @param application Pointer to parent application.
         */
        explicit GlobalThumbnails(HG::Editor::Application* application);

        /**
         * @brief Destructor.
         */
        ~GlobalThumbnails();

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Editor::Application* application() const;

        /**
         * @brief Method for getting handle to global thumbnail.
         * If there is no such thumbnail or it was not loaded
         * yet - InvalidHandle will be returned.
         * @param thumb Identifier of thumbnail.
         * @return Handle to thumbnail or InvalidHandle.
         */
        HG::Editor::ThumbnailsCache::Handle getHandle(Thumbs thumb);

        /**
         * @brief Method that performs loading of
         * global thumbnails. This thumbnails will
         * be freed at destructor.
         * This call creates some textures, that
         * has to be freed with `finishLoading`
         */
        void startLoading();

        /**
         * @brief Method that performs dispose of
         * allocated textures. This call must be performed
         * after cache invalidation.
         */
        void finishLoading();

    private:

        /**
         * @brief Method, that performs loading of
         * thumbnail from file, creates texture
         * and pushes it to thumbnails cache.
         * @param thumb Thumbnail identifier.
         * @param path Path to file.
         */
        void loadInternal(Thumbs thumb, std::filesystem::path path);

        std::unordered_map<
            Thumbs,
            HG::Editor::ThumbnailsCache::Handle
        > m_handles;

        std::vector<HG::Rendering::Base::Texture*> m_textures;

        HG::Editor::Application* m_application;
    };
}


