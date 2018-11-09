// Editor
#include <Widgets/OpenPath.hpp>
#include <Editor/Application.hpp>
#include <Tools/ImGuiWidgets.hpp>

// C++ STL
#include <algorithm>

// HG::Core
#include <HG/Core/ResourceManager.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Surface.hpp>
#include <HG/Utils/Loaders/STBImageLoader.hpp>

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
    m_settings(),
    m_currentOpenState(false)
{
    m_inputBuffer.reserve(max_path);
    setOpened(false);
    clear();
}

HG::Editor::Widgets::OpenPath::~OpenPath()
{
    delete m_file;
    delete m_directory;
}

void HG::Editor::Widgets::OpenPath::onInitialization()
{
    m_file = new HG::Rendering::Base::Texture(
        application()
            ->resourceManager()
            ->load<HG::Utils::STBImageLoader>("images/text.png")
    );

    m_directory = new HG::Rendering::Base::Texture(
        application()
            ->resourceManager()
            ->load<HG::Utils::STBImageLoader>("images/folder.png")
    );
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
    // todo: When ImGui will be ready - fix this strange behaviour
    if (m_currentOpenState != isOpened())
    {
        m_currentOpenState = isOpened();

        if (isOpened())
        {
            ImGui::OpenPopup("Explorer");
        }
    }

    ImGui::SetNextWindowSize({600, 500}, ImGuiCond_FirstUseEver);
    if (ImGui::BeginPopupModal("Explorer"))
    {
        if (!isOpened())
        {
            ImGui::CloseCurrentPopup();
        }

        drawButtonsPath();

        drawItemsChild();

        drawFileInput();

        drawButtons();

        ImGui::EndPopup();
    }
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
    m_currentPath = "/home";
    updateFilesInCurrentPath();
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
        if ((!m_settings.showHidden &&
             isHidden(file)) ||

            (file.status.type() == std::filesystem::file_type::regular &&
             m_settings.filterByExtension &&
             !m_settings.fileTypes.empty() &&
             file.path.has_extension() &&
             m_settings.fileTypes[m_settings.currentFileType] != file.path.extension()))
        {
            continue;
        }

        HG::Rendering::Base::Texture* icon = nullptr;

        if (file.status.type() == std::filesystem::file_type::directory)
        {
            icon = m_directory;
        }
        else if (file.status.type() == std::filesystem::file_type::regular)
        {
            icon = m_file;
        }

        auto isSelected = file.path == m_selected;

        if (ImGui::IconSelectable(file.path.filename().c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick, {0, 0}, icon))
        {
            if (validateData(file))
            {
                m_selected = file.path;
                m_inputBuffer = file.path.filename();
                m_inputBuffer.reserve(max_path);
            }

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
    float comboWidth = 100;

    ImGui::Text("Filename:");

    ImGui::SameLine();

    ImGui::PushItemWidth(-(comboWidth + ImGui::GetStyle().ItemSpacing.x));
    if (ImGui::InputText(
            "##FilenameInput_OpenPath_Input",
            m_inputBuffer.data(),
            m_inputBuffer.capacity(),
            ImGuiInputTextFlags_EnterReturnsTrue
    ))
    {
        FileData data;

        data.path  = m_currentPath / m_inputBuffer.c_str();

        if (data.proceedStatus())
        {
            if (((data.status.type() == std::filesystem::file_type::directory &&
                  m_settings.mode == Settings::Mode::Directory) ||
                 (data.status.type() == std::filesystem::file_type::regular &&
                  m_settings.mode == Settings::Mode::File) ||
                  m_settings.mode == Settings::Mode::Any) &&
                  validateData(data) &&
                 (m_settings.additionalChecker == nullptr ||
                  m_settings.additionalChecker(data)))
            {
                if (m_callback)
                {
                    m_callback(m_selected);
                }

                clear();
                setOpened(false);
                return;
            }

            if (data.status.type() == std::filesystem::file_type::directory)
            {
                m_currentPath = data.path;
                updateFilesInCurrentPath();
            }
            else
            {
                m_inputBuffer = m_selected.filename();
            }
        }
        else
        {
            m_inputBuffer = m_selected.filename();
        }
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();

    if (m_settings.mode != Settings::Mode::Directory &&
        !m_settings.fileTypes.empty())
    {
        ImGui::PushItemWidth(comboWidth);
        ImGui::Combo(
            "##ExtensionSelection_OpenPath_Input",
            &m_settings.currentFileType,
            [](void* data, int index, const char** value) -> bool
            {
                auto settings = static_cast<Settings*>(data);

                if (index >= settings->fileTypes.size())
                {
                    return false;
                }

                (*value) = settings->fileTypes[index].c_str();

                return true;
            },
            &m_settings,
            static_cast<int>(m_settings.fileTypes.size())
        );
        ImGui::PopItemWidth();
    }
}

void HG::Editor::Widgets::OpenPath::drawButtons()
{
    if (ImGui::Button("Cancel##OpenPath_Buttons"))
    {
        clear();
        setOpened(false);
    }

    ImGui::SameLine();

    if (ImGui::Button("Ok##OpenPath_Buttons"))
    {
        if (m_callback)
        {
            m_callback(m_selected);
        }

        clear();
        setOpened(false);
    }
}

bool HG::Editor::Widgets::OpenPath::validateData(const HG::Editor::Widgets::OpenPath::FileData &data)
{
    switch (m_settings.mode)
    {
    case Settings::Mode::Directory:
        return data.status.type() == std::filesystem::file_type::directory  &&
               (m_settings.additionalChecker == nullptr ||
                m_settings.additionalChecker(data));
    case Settings::Mode::File:
    {
        if (data.status.type() != std::filesystem::file_type::regular)
        {
            return false;
        }

        return (m_settings.fileTypes.empty() ||
                hasProperExtension(data)) &&
               (m_settings.additionalChecker == nullptr ||
                m_settings.additionalChecker(data));
    }
    case Settings::Mode::Any:
        return m_settings.additionalChecker == nullptr ||
               m_settings.additionalChecker(data);

    }

    return true;
}

bool HG::Editor::Widgets::OpenPath::isHidden(const HG::Editor::Widgets::OpenPath::FileData &data)
{
    auto filename = data.path.filename();

    return filename.empty() ||
           filename.string()[0] == '.';
}

bool HG::Editor::Widgets::OpenPath::hasProperExtension(const HG::Editor::Widgets::OpenPath::FileData &data)
{
    return data.path.has_extension() &&
           std::find(
               m_settings.fileTypes.begin(),
               m_settings.fileTypes.end(),
               data.path.extension().string()
           ) != m_settings.fileTypes.end();
}
