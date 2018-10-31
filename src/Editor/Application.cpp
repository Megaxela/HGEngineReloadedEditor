// Editor
#include <Editor/Application.hpp>
#include <Editor/ProjectController.hpp>
#include <Fabrics/AssetsFabric.hpp>
#include <Fabrics/PropertyEditorsFabric.hpp>

HG::Editor::Application::Application(std::string name, int argc, char **argv) :
    HG::Core::Application(std::move(name), argc, argv),
    m_projectController(new HG::Editor::ProjectController(this)),
    m_propertyEditorsFabric(new HG::Editor::Fabrics::PropertyEditorsFabric()),
    m_assetsFabric(new HG::Editor::Fabrics::AssetsFabric())
{

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
