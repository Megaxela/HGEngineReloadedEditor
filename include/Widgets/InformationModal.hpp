#pragma once

// C++ STL
#include <vector>

// Editor
#include <Widgets/AbstractWidget.hpp>
#include <Tools/ThumbnailsCache.hpp>

namespace HG::Rendering::Base
{
    class Texture;
}

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes simple
     * modal window for errors, warnings and info messages.
     */
    class InformationModal : public AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        InformationModal();

        /**
         * @brief Destructor.
         */
        ~InformationModal() override;

        /**
         * @brief Method, that will show modal window
         * with info severity and provided text.
         * @param text Text.
         */
        void info(std::string text);

        /**
         * @brief Method, that will show modal window
         * with warning severity and provided text.
         * @param text Text.
         */
        void warning(std::string text);

        /**
         * @brief Method, that will show modal window
         * with error severity and provided text.
         * @param text Text.
         */
        void error(std::string text);

    protected:

        /**
         * @brief Method for drawing widget.
         */
        void onDraw() override;

        /**
         * @brief Method that performs loading icons
         * and prepares them for thumbnails caching.
         */
        void onInitialization() override;

        /**
         * @brief Method that performs icons delete
         * because they will be loaded to thumbnails cache.
         */
        void onPostInitialization() override;

    private:

        enum class Type
        {
            Info,
            Warning,
            Error
        };

        HG::Editor::ThumbnailsCache::Handle m_infoIconHandle;
        HG::Editor::ThumbnailsCache::Handle m_warningIconHandle;
        HG::Editor::ThumbnailsCache::Handle m_errorIconHandle;

        std::vector<HG::Rendering::Base::Texture*> m_resourcesToFree;

        bool m_currentOpenState;

        Type m_type;
        std::string m_text;
    };
}


