#pragma once

// C++ STL
#include <unordered_map>
#include <vector>

// GLM
#include <glm/vec2.hpp>
#include <HG/Rendering/Base/BlitData.hpp>

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
     * @todo Make this class thread safe
     */
    class ThumbnailsCache
    {
    public:

        using Handle = std::size_t;

        static constexpr Handle InvalidHandle = static_cast<const Handle>(-1);

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
        HG::Editor::Application* application() const;

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
        Handle addThumbnail(HG::Rendering::Base::Texture* texture, glm::ivec2 size={0, 0});

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
        void updateThumbnail(Handle handler, HG::Rendering::Base::Texture* texture, glm::ivec2 size={0, 0});

        /**
         * @brief Method for removing thumbnail.
         * This method will remove thumbnail from cache
         * regardless whether `invalidateCache` was called.
         * @param handler Handler.
         */
        void removeThumbnail(Handle handler);

        /**
         * @brief Method for checking is thumbnail loaded
         * to cache.
         * @param handler Thumbnail handler.
         */
        bool isAvailable(Handle handler) const;

        /**
         * @brief Method for getting thumbnail TLBR
         * value. If thumbnail is unavailable
         * `std::runtime_error` will be thrown.
         * @param handler
         * @return
         */
        TLBR thumbnailTLBR(Handle handler) const;

        /**
         * @brief Method for notifying that cache has to be
         * rebuilt.
         */
        void invalidateCache();

        /**
         * @brief Method, that performs convertation
         * from pixel values in TLBR to UV values.
         * If cache does not have set up internal texture -
         * glm::vec(0, 0) will be returned.
         * @param value TL/BR value.
         * @return Converted to UV.
         */
        glm::vec2 pixelsToUV(glm::ivec2 value);

    private:

        using HandlerTLBRContainer = std::unordered_map<HG::Editor::ThumbnailsCache::Handle, HG::Editor::ThumbnailsCache::TLBR>;

        /**
         * @brief Method, that performs preparing current thumbnails
         * for loading to GPU.
         */
        void prepareCurrentThumbnails(HG::Rendering::Base::BlitData &blitData,
                                      const HG::Editor::ThumbnailsCache::HandlerTLBRContainer &newCurrent);
        /**
         * @brief Method, that performs preparing new thumbnails
         * for loading to GPU.
         */
        void prepareNewThumbnails(HG::Rendering::Base::BlitData &blitData,
                                  const HG::Editor::ThumbnailsCache::HandlerTLBRContainer &newCurrent);

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
        HandlerTLBRContainer m_currentThumbnails;
        std::unordered_map<Handle, NewThumbnail> m_newOrUpdateThumbnails;
        Handle m_handlerCounter;
    };
}


