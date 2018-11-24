#pragma once

// C++ STL
#include <unordered_map>
#include <vector>

// GLM
#include <glm/vec2.hpp>

namespace HG::Rendering::Base
{
    class Texture;
    class RenderTarget;
}

namespace HG::Editor
{
    class Application;

    /**
     * @brief Class, that describes cache for thumbnails.
     * This class performs registration/deregistration of
     * thumbnails and icons.
     */
    class ThumbnailsCache
    {
    public:

        using Handler = std::size_t;

        struct TLBR
        {
            glm::ivec2 tl;
            glm::ivec2 br;
        };

        /**
         * @brief Constructor.
         * @param application Pointer to parent application.
         */
        explicit ThumbnailsCache(HG::Editor::Application* application);

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Editor::Application* parentApplication() const;

        /**
         * @brief Method for getting pointer to texture
         * with thumbnails.
         * @return Pointer to texture.
         */
        HG::Rendering::Base::Texture* texture() const;

        /**
         * @brief Method for adding thumbnail to cache.
         * It will not be available until invalidate
         * cache is called.
         * @param texture Thumbnail texture.
         * @param size Thumbnail result size. (texture
         * will be resized). If size is {0, 0} size
         * will be taken from texture.
         */
        Handler addThumbnail(HG::Rendering::Base::Texture* texture, glm::ivec2 size={0, 0});

        /**
         * @brief Method for updating thumbnail on existing handler.
         * If handler will not exist - `std::invalid_argument` exception
         * will be thrown.
         * @param handler Thumbnail handler.
         * @param texture New thumbnail texture.
         * @param size New thumbnail size. (texture
         * will be resized). If size is {0, 0} size
         * will be taken from texture.
         */
        void updateThumbnail(Handler handler, HG::Rendering::Base::Texture* texture, glm::ivec2 size={0, 0});

        /**
         * @brief Method for removing thumbnail.
         * This method will remove thumbnail from cache
         * regardless whether `invalidateCache` was called.
         * @param handler Handler.
         */
        void removeThumbnail(Handler handler);

        /**
         * @brief Method for checking is thumbnail loaded
         * to cache.
         * @param handler Thumbnail handler.
         */
        bool isAvailable(Handler handler) const;

        /**
         * @brief Method for getting thumbnail TLBR
         * value. If thumbnail is unavailable
         * `std::runtime_error` will be thrown.
         * @param handler
         * @return
         */
        TLBR thumbnailTLBR(Handler handler) const;

        /**
         * @brief Method for notifying that cache has to be
         * rebuilt.
         */
        void invalidateCache();

    private:

        /**
         * @brief Method, that performs loading current thumbnails
         * to GPU.
         */
        void loadCurrentThumbnails(HG::Rendering::Base::RenderTarget* texture,
                                   const std::unordered_map<Handler, TLBR>& newCurrent);

        struct NewThumbnail
        {
            NewThumbnail() :
                texture(nullptr),
                size({0, 0})
            {}

            NewThumbnail(HG::Rendering::Base::Texture* texture,
                         glm::ivec2 size) :
                texture(texture),
                size(size)
            {}

            HG::Rendering::Base::Texture* texture;
            glm::ivec2 size;
        };

        HG::Editor::Application* m_parentApplication;

        HG::Rendering::Base::Texture* m_texture;
        std::unordered_map<Handler, TLBR> m_currentThumbnails;
        std::unordered_map<Handler, NewThumbnail> m_newOrUpdateThumbnails;
        Handler m_handlerCounter;
    };
}


