// Editor
#include <Widgets/AbstractWidget.hpp>

HG::Editor::Widgets::AbstractWidget::AbstractWidget() :
    m_opened(true),
    m_application(nullptr)
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

void HG::Editor::Widgets::AbstractWidget::setApplication(HG::Editor::Application *application)
{
    m_application = application;
}

HG::Editor::Application *HG::Editor::Widgets::AbstractWidget::application() const
{
    return m_application;
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