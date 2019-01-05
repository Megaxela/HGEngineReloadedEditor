// Editor
#include <Widgets/AbstractWidget.hpp>
#include <EditBehaviours/GraphicsInterface.hpp>

HG::Editor::Widgets::AbstractWidget::AbstractWidget() :
    m_opened(true),
    m_graphicsInterface(nullptr)
{

}

void HG::Editor::Widgets::AbstractWidget::setOpened(bool opened)
{
    m_opened = opened;
}

bool HG::Editor::Widgets::AbstractWidget::isOpened() const
{
    return m_opened;
}

void HG::Editor::Widgets::AbstractWidget::draw()
{
    onDraw();
}

void HG::Editor::Widgets::AbstractWidget::update()
{
    onUpdate();
}

void HG::Editor::Widgets::AbstractWidget::setGraphicsInterface(Behaviours::GraphicsInterface* graphicsInterface)
{
    m_graphicsInterface = graphicsInterface;
}

HG::Editor::Application *HG::Editor::Widgets::AbstractWidget::application() const
{
    if (m_graphicsInterface == nullptr)
    {
        return nullptr;
    }

    return m_graphicsInterface->editorApplication();
}

HG::Editor::Behaviours::GraphicsInterface* HG::Editor::Widgets::AbstractWidget::graphicsInterface() const
{
    return m_graphicsInterface;
}

void HG::Editor::Widgets::AbstractWidget::onUpdate()
{

}

void HG::Editor::Widgets::AbstractWidget::initialize()
{
    onInitialization();
}

void HG::Editor::Widgets::AbstractWidget::onInitialization()
{

}