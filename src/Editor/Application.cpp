// Editor
#include <Editor/Application.hpp>
#include <Editor/ProjectController.hpp>
#include <Fabrics/AssetsFabric.hpp>
#include <Fabrics/PropertyEditorsFabric.hpp>
#include <AssetSystem/AssetsManager.hpp>
#include <Tools/ThumbnailsCache.hpp>

HG::Editor::Application::Application(std::string name, int argc, char **argv) :
    HG::Core::Application(std::move(name), argc, argv),
    m_projectController(new HG::Editor::ProjectController(this)),
    m_thumbnailsCache(new HG::Editor::ThumbnailsCache(this)),
    m_propertyEditorsFabric(new HG::Editor::Fabrics::PropertyEditorsFabric()),
    m_assetsFabric(new HG::Editor::Fabrics::AssetsFabric())
{
    m_propertyEditorsFabric->registrateDefault();
}

HG::Editor::Application::~Application()
{
    delete m_projectController;
    delete m_propertyEditorsFabric;
    delete m_assetsFabric;
}

HG::Editor::ProjectController *HG::Editor::Application::projectController() const
{
    return m_projectController;
}

HG::Editor::Fabrics::PropertyEditorsFabric *HG::Editor::Application::propertyEditorsFabric() const
{
    return m_propertyEditorsFabric;
}

HG::Editor::Fabrics::AssetsFabric *HG::Editor::Application::assetsFabric() const
{
    return m_assetsFabric;
}

HG::Editor::ThumbnailsCache *HG::Editor::Application::thumbnailsCache() const
{
    return m_thumbnailsCache;
}

bool HG::Editor::Application::performCycle()
{
    m_projectController->assetManager()->proceedEvents();

    return HG::Core::Application::performCycle();
}
