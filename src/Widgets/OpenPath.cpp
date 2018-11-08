// Editor
#include <Widgets/OpenPath.hpp>

// C++ STL
#include <algorithm>

// ALogger
#include <CurrentLogger.hpp>

// ImGui
#include <imgui.h>

namespace {
    constexpr std::size_t max_path = 2048;
}

bool HG::Editor::Widgets::OpenPath::FileData::proceedStatus()
{
    std::error_code ec;
    status = std::filesystem::status(path, ec);

    return !bool(ec);
}

HG::Editor::Widgets::OpenPath::OpenPath() :
    m_settings()
{
    m_currentPath = "/home/ushanovalex";
    updateFilesInCurrentPath();
}

void HG::Editor::Widgets::OpenPath::setOkCallback(HG::Editor::Widgets::OpenPath::OkCallback callback)
{
    m_callback = std::move(callback);
}

HG::Editor::Widgets::OpenPath::Settings &HG::Editor::Widgets::OpenPath::settings()
{
    return m_settings;
}

void HG::Editor::Widgets::OpenPath::onDraw()
{
    if (ImGui::Begin("OpenPath", &m_opened))
    {
        drawButtonsPath();

        drawItemsChild();

        drawFileInput();

        drawButtons();
    }
    ImGui::End();
}

void HG::Editor::Widgets::OpenPath::updateFilesInCurrentPath()
{
    m_files.clear();

    for (auto& p : std::filesystem::directory_iterator(
            m_currentPath,
            std::filesystem::directory_options::skip_permission_denied
    ))
    {
        FileData cache;

        cache.path = p;
        cache.proceedStatus();

        m_files.emplace_back(std::move(cache));
    }

    std::sort(
            m_files.begin(),
            m_files.end(),
            [](const FileData& l, const FileData& r) -> bool
            {
                return l.path < r.path;
            }
    );
}

void HG::Editor::Widgets::OpenPath::clear()
{

}

void HG::Editor::Widgets::OpenPath::drawButtonsPath()
{
    std::filesystem::path pathCache;

    bool changed = false;

    for (auto&& segment : m_currentPath)
    {
        if (segment != *m_currentPath.begin())
        {
            ImGui::SameLine();
        }

        pathCache /= segment;
        auto segmentName = segment.string() + "##OpenPath_Path";

        if (ImGui::Button(segmentName.c_str()))
        {
            m_currentPath = pathCache;
            changed = true;
            break;
        }
    }

    if (changed)
    {
        updateFilesInCurrentPath();
    }
}

void HG::Editor::Widgets::OpenPath::drawItemsChild()
{
    auto size = ImGui::GetContentRegionAvail();

    size.y -= ImGui::GetItemsLineHeightWithSpacing() * 2;

    ImGui::BeginChild("##ItemsChild_OpenPath_Items", size);

    bool changed = false;

    if (std::distance(m_currentPath.begin(), m_currentPath.end()) > 1 &&
        ImGui::Selectable("..##GotoBack_OpenPath_Items", m_currentPath.parent_path() == m_selected, ImGuiSelectableFlags_AllowDoubleClick))
    {
        if (ImGui::IsMouseDoubleClicked(0))
        {
            m_currentPath = m_currentPath.parent_path();
            changed = true;
        }
    }

    for (const auto& file : m_files)
    {
        auto isSelected = file.path == m_selected;

        if (ImGui::Selectable(file.path.filename().c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick))
        {
            m_selected = file.path;

            if (ImGui::IsMouseDoubleClicked(0) &&
                file.status.type() == std::filesystem::file_type::directory)
            {
                m_currentPath = file.path;
                changed = true;
            }
        }
    }

    if (changed)
    {
        updateFilesInCurrentPath();
    }

    ImGui::EndChild();
}

void HG::Editor::Widgets::OpenPath::drawFileInput()
{
    static char buffer[2048];

    const float comboWidth = 100;

    ImGui::Text("Filename:");

    ImGui::SameLine();

    ImGui::PushItemWidth(-(comboWidth + ImGui::GetStyle().ItemSpacing.x));
    ImGui::InputText("##FilenameInput_OpenPath_Input", buffer, 2048);
    ImGui::PopItemWidth();

    ImGui::SameLine();

    static const char* values[] = {".cpp", ".hpp", ".gcc"};
    static int current = 0;

    ImGui::PushItemWidth(comboWidth);
    ImGui::Combo("##ExtensionSelection_OpenPath_Input", &current, values, 3);
    ImGui::PopItemWidth();
}

void HG::Editor::Widgets::OpenPath::drawButtons()
{
    ImGui::Button("Cancel##OpenPath_Buttons");
    ImGui::SameLine();
    ImGui::Button("Ok##OpenPath_Buttons");
}
