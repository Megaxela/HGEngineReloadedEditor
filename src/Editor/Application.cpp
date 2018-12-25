// Editor
#include <Editor/Application.hpp>
#include <Editor/ProjectController.hpp>
#include <Fabrics/AssetsFabric.hpp>
#include <Fabrics/PropertyEditorsFabric.hpp>
#include <AssetSystem/AssetsManager.hpp>
#include <Tools/ThumbnailsCache.hpp>
#include <Editor/ShortcutsProcessor.hpp>
#include <Tools/GlobalThumbnails.hpp>

HG::Editor::Application::Application(std::string name, int argc, char **argv) :
    HG::Core::Application(std::move(name), argc, argv),
    m_projectController(new HG::Editor::ProjectController(this)),
    m_thumbnailsCache(new HG::Editor::ThumbnailsCache(this)),
    m_shortcutsProcessor(new HG::Editor::ShortcutsProcessor()),
    m_globalThumbnails(new HG::Editor::GlobalThumbnails(this)),
    m_propertyEditorsFabric(new HG::Editor::Fabrics::PropertyEditorsFabric()),
    m_assetsFabric(new HG::Editor::Fabrics::AssetsFabric())
{
    m_propertyEditorsFabric->registrateDefault();
    m_assetsFabric->registrateDefault();
}

HG::Editor::Application::~Application()
{
    delete m_globalThumbnails;
    delete m_projectController;
    delete m_thumbnailsCache;
    delete m_shortcutsProcessor;
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

HG::Editor::ShortcutsProcessor* HG::Editor::Application::shortcutsProcessor() const
{
    return m_shortcutsProcessor;
}

HG::Editor::GlobalThumbnails* HG::Editor::Application::globalThumbnails() const
{
    return m_globalThumbnails;
}

bool HG::Editor::Application::init()
{
    auto result = HG::Core::Application::init();

    if (!result)
    {
        return false;
    }

    m_globalThumbnails->startLoading();

    return true;
}

bool HG::Editor::Application::performCycle()
{
    m_shortcutsProcessor->proceed(input());

    m_projectController->assetManager()->proceedEvents();

    auto result = HG::Core::Application::performCycle();

    if (m_thumbnailsCache->invalidationRequired())
    {
        m_thumbnailsCache->invalidateCache();
    }

    m_globalThumbnails->finishLoading();

    return result;
}
