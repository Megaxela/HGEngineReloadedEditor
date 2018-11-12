#pragma once

// HG::Core
#include <HG/Core/Application.hpp>

namespace HG::Editor::Fabrics
{
    class PropertyEditorsFabric;
    class AssetsFabric;
}

namespace HG::Editor
{
    class ProjectController;

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

        bool performCycle() override;

    private:

        HG::Editor::ProjectController* m_projectController;

        HG::Editor::Fabrics::PropertyEditorsFabric* m_propertyEditorsFabric;
        HG::Editor::Fabrics::AssetsFabric* m_assetsFabric;
    };
}


