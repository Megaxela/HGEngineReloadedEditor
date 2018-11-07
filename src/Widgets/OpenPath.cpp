// Editor
#include <Widgets/OpenPath.hpp>

// C++ STL
#include <algorithm>

// ImGui
#include <imgui.h>

namespace {
    constexpr std::size_t max_path = 2048;
}

bool HG::Editor::Widgets::OpenPath::FileCache::proceedStatus()
{
    std::error_code ec;
    status = std::filesystem::status(path, ec);

    return !bool(ec);
}

bool HG::Editor::Widgets::OpenPath::FileCache::proceedChildren()
{
    if (status.type() != std::filesystem::file_type::directory)
    {
        isProceed = true;

        return true;
    }

    if (path == "/dev/fd" ||
        path == "/proc/self")
    {
        isProceed = true;
        return true;
    }

    for (auto& p : std::filesystem::directory_iterator(
            path,
            std::filesystem::directory_options::skip_permission_denied
    ))
    {
        FileCache cache;

        cache.path = p;
        cache.proceedStatus();

        children.emplace_back(std::move(cache));
    }

    std::sort(
        children.begin(),
        children.end(),
        [](FileCache& l, FileCache& r) -> bool
        {
            return l.path < r.path;
        }
    );

    isProceed = true;

    return true;
}

HG::Editor::Widgets::OpenPath::OpenPath() :
    m_root(),
    m_pathBuffer()
{
    m_pathBuffer.reserve(2048);

    initialize();
}

void HG::Editor::Widgets::OpenPath::onDraw()
{
    if (ImGui::Begin("OpenPath", &m_opened))
    {
        drawToolbar();

        drawPathEditor();

        drawTreeWidget();
    }
    ImGui::End();
}

void HG::Editor::Widgets::OpenPath::initialize()
{
    // todo: Add initialization for windows
    m_root.path = "/";
    m_root.proceedStatus();
    m_root.proceedChildren();
    m_pathBuffer = "/";

    assert(m_pathBuffer.capacity() == max_path);
}

void HG::Editor::Widgets::OpenPath::drawToolbar()
{

}

void HG::Editor::Widgets::OpenPath::drawPathEditor()
{
    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
    ImGui::InputText("##OpenPath/Path", m_pathBuffer.data(), max_path);
    ImGui::PopItemWidth();
}

void HG::Editor::Widgets::OpenPath::drawTreeWidget()
{
    ImGui::BeginChildFrame(1, ImGui::GetContentRegionAvail());

    auto flags =
         (m_root.children.empty() ? ImGuiTreeNodeFlags_Leaf : 0U) |
         ImGuiTreeNodeFlags_OpenOnArrow;

    // Drawing root
    if (ImGui::TreeNodeEx("Root##OpenPath/RootElement", flags))
    {
        for (auto&& child : m_root.children)
        {
            displayPath(&child);
        }

        ImGui::TreePop();
    }

    ImGui::EndChildFrame();
}

void HG::Editor::Widgets::OpenPath::displayPath(FileCache* fileCache)
{
    if (!fileCache->isProceed)
    {
        fileCache->proceedChildren();
    }

    auto hasChildren = !fileCache->children.empty();

    auto nodeFlags =
            (hasChildren ? 0U : ImGuiTreeNodeFlags_Leaf) |
            ImGuiTreeNodeFlags_OpenOnArrow |
            (fileCache->path == m_selectedPath ? ImGuiTreeNodeFlags_Selected : 0U);

    bool opened = ImGui::TreeNodeEx(fileCache, nodeFlags, "%s", fileCache->path.filename().c_str());

    if (ImGui::IsItemClicked(0))
    {
        m_selectedPath = fileCache->path;
        m_pathBuffer = m_selectedPath.string();
    }

    if (opened)
    {
        for (auto&& childTransform : fileCache->children)
        {
            displayPath(&childTransform);
        }

        ImGui::TreePop();
    }
}
