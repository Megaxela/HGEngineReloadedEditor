#pragma once

// C++ STL
#include <filesystem>

// Editor
#include <Widgets/AbstractWidget.hpp>

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes widget for selecting
     * one or multiple paths (files or paths)
     */
    class OpenPath : public AbstractWidget
    {
    public:
        OpenPath();

    protected:
        void onDraw() override;

    private:

        struct FileCache
        {
            std::filesystem::path path;
            bool isProceed = false;
            std::filesystem::file_status status;
            std::vector<FileCache> children;

            bool proceedStatus();

            bool proceedChildren();
        };

        void initialize();

        void drawToolbar();

        void drawPathEditor();

        void drawTreeWidget();

        void displayPath(FileCache* fileCache);

        FileCache m_root;

        std::string m_pathBuffer;
        std::filesystem::path m_selectedPath;
    };
}