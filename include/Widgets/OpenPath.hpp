#pragma once

// C++ STL
#include <filesystem>
#include <functional>

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

        using OkCallback = std::function<void(std::filesystem::path)>;

        struct Settings
        {
            bool showHidden = false;
        };

        /**
         * @brief Constructor.
         */
        OpenPath();

        /**
         * @brief Method for getting widget settings for
         * editing.
         * @return Reference to settings.
         */
        Settings& settings();

        /**
         * @brief Method for setting ok callback.
         * @param callback Callback.
         */
        void setOkCallback(OkCallback callback);

        /**
         * @brief Method for clearing internal
         */
        void clear();

    protected:
        void onDraw() override;

    private:

        struct FileData
        {
            std::filesystem::path path;
            std::filesystem::file_status status;
            bool proceedStatus();
        };

        void drawButtonsPath();

        void drawItemsChild();

        void drawFileInput();

        void drawButtons();

        void updateFilesInCurrentPath();

        std::vector<FileData> m_files;

        std::filesystem::path m_currentPath;
        std::filesystem::path m_selected;

        OkCallback m_callback;

        Settings m_settings;
    };
}