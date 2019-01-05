// C++ STL
#include <memory>

// Editor
#include <Widgets/LineInputModal.hpp>
#include <Tools/ImGuiWidgets.hpp>
#include <Tools/ImGuiIdentificators.hpp>

HG::Editor::Widgets::LineInputModal::LineInputModal() :
    m_buffer(),
    m_text(),
    m_successCallback(),
    m_cancelCallback(),
    m_currentOpenState(false)
{
    setOpened(false);
    m_buffer[0] = '\0';
}

HG::Editor::Widgets::LineInputModal::~LineInputModal()
{

}

void HG::Editor::Widgets::LineInputModal::setOnSuccessCallback(HG::Editor::Widgets::LineInputModal::SuccessCallback callback)
{
    m_successCallback = std::move(callback);
}

void HG::Editor::Widgets::LineInputModal::setOnCancelCallback(HG::Editor::Widgets::LineInputModal::CancelCallback callback)
{
    m_cancelCallback = std::move(callback);
}

void HG::Editor::Widgets::LineInputModal::execute(const std::string& value, std::string text)
{
    memcpy(m_buffer, value.c_str(), std::min(MaxLength, value.size()));
    m_text = std::move(text);
    setOpened(true);
}

void HG::Editor::Widgets::LineInputModal::onDraw()
{
    ImGui::IDGuard idGuard(HG::ID::LineInputModal::Window);

    if (m_currentOpenState != isOpened())
    {
        m_currentOpenState = isOpened();

        if (isOpened())
        {
            ImGui::OpenPopup(HG::Names::LineInputModal::Window);
        }
    }

//    ImGui::SetNextWindowSize({300, 200}, ImGuiCond_FirstUseEver);
    if (ImGui::BeginPopupModal(HG::Names::LineInputModal::Window, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (!isOpened())
        {
            cleanAndClose();
        }

        if (!m_text.empty())
        {
            ImGui::TextWrapped("%s", m_text.c_str());
        }

        auto entered = ImGui::InputText("", m_buffer, MaxLength, ImGuiInputTextFlags_EnterReturnsTrue);

        if (ImGui::Button(HG::Names::LineInputModal::Button::Ok) ||
            entered)
        {
            if (m_successCallback)
            {
                m_successCallback(std::string(m_buffer));
            }

            cleanAndClose();
        }

        ImGui::SameLine();

        if (ImGui::Button(HG::Names::LineInputModal::Button::Cancel))
        {
            if (m_cancelCallback)
            {
                m_cancelCallback();
            }

            cleanAndClose();
        }

        ImGui::EndPopup();
    }
}

void HG::Editor::Widgets::LineInputModal::cleanAndClose()
{
    ImGui::CloseCurrentPopup();
    m_buffer[0] = '\0';
    m_cancelCallback = nullptr;
    m_successCallback = nullptr;
    setOpened(false);
}
