// Editor
#include <Widgets/Logging.hpp>
#include <Tools/ImGuiWidgets.hpp>
#include <Tools/ImGuiIdentificators.hpp>

// ALogger
#include <LogsListener.hpp>

// ImGui
#include <imgui.h>

HG::Editor::Widgets::Logging::Logging() :
    m_logsListener(nullptr),
    m_messagesBuffer()
{

}

void HG::Editor::Widgets::Logging::setLogsListener(Logger::LogsListenerPtr logsListener)
{
    m_logsListener = std::move(logsListener);
}

Logger::LogsListenerPtr HG::Editor::Widgets::Logging::logsListener() const
{
    return m_logsListener;
}

void HG::Editor::Widgets::Logging::onDraw()
{
    ImGui::IDGuard idGuard(HG::ID::Logging::Window);

    if (ImGui::Begin(HG::Names::Logging::Window, &m_opened))
    {
        for (auto&& message : m_messagesBuffer)
        {
            ImGui::Selectable("%s", message.message.c_str());
        }
    }
    ImGui::End();
}

void HG::Editor::Widgets::Logging::onUpdate()
{
    while (m_logsListener->hasMessages())
    {
        m_messagesBuffer.push_back(m_logsListener->popMessage());
    }
}
