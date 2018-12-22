#pragma once

// HG::Core
#include <HG/Core/Application.hpp>
#include "ShortcutsProcessor.hpp"

namespace HG::Editor::Fabrics
{
    class PropertyEditorsFabric;
    class AssetsFabric;
}

namespace HG::Editor
{
    class ProjectController;
    class ThumbnailsCache;
    class ShortcutProcessor;

    /**
     * @brief Class that describes editor application
     * instance.
     */
    class Application : public HG::Core::Application
    {
    public:
        /**
         * @brief Constructor.
         * @param name Window title.
         */
        explicit Application(std::string name, int argc, char **argv);

        /**
         * @brief Destructor.
         */
        ~Application() override;

        /**
         * @brief Method for getting pointer to project controller.
         * @return Pointer to project controller.
         */
        HG::Editor::ProjectController* projectController() const;

        /**
         * @brief Method for getting pointer to property editors fabric.
         * @return Pointer to property editors fabric.
         */
        HG::Editor::Fabrics::PropertyEditorsFabric* propertyEditorsFabric() const;

        /**
         * @brief Method for getting pointer to assets fabric.
         * @return Pointer to assets fabric.
         */
        HG::Editor::Fabrics::AssetsFabric* assetsFabric() const;

        /**
         * @brief Method for getting pointer to application thumbnails cache.
         * @return Pointer to thumbnails cache.
         */
        HG::Editor::ThumbnailsCache* thumbnailsCache() const;

        /**
         * @brief Method for getting pointer to shortcuts processor
         * object.
         * @return Pointer to shortcuts processor.
         */
        HG::Editor::ShortcutsProcessor* shortcutsProcessor() const;

        /**
         * @brief Overridden method for performing single
         * application cycle.
         * @return Success.
         */
        bool performCycle() override;

    private:

        HG::Editor::ProjectController* m_projectController;

        HG::Editor::ThumbnailsCache* m_thumbnailsCache;
        HG::Editor::ShortcutsProcessor* m_shortcutsProcessor;

        HG::Editor::Fabrics::PropertyEditorsFabric* m_propertyEditorsFabric;
        HG::Editor::Fabrics::AssetsFabric* m_assetsFabric;
    };
}


