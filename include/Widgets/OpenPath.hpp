#pragma once

// C++ STL
#include <filesystem>
#include <functional>

// Editor
#include <Widgets/AbstractWidget.hpp>

namespace HG::Rendering::Base
{
    class Texture;
}

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes widget for selecting
     * one or multiple paths (files or paths)
     */
    class OpenPath : public AbstractWidget
    {
    public:

        struct FileData
        {
            std::filesystem::path path;
            std::filesystem::file_status status;
            bool proceedStatus();
        };

        using OkCallback = std::function<void(std::filesystem::path)>;

        struct Settings
        {
            enum class Mode
            {
                Any,
                File,
                Directory
            };

            bool showHidden = false;
            bool filterByExtension = true;
            Mode mode = Mode::Any;
            std::function<bool(const FileData& data)> additionalChecker;
            std::vector<std::string> fileTypes;
            int currentFileType = 0;
        };

        /**
         * @brief Constructor.
         */
        OpenPath();

        /**
         * @brief Destructor.
         */
        ~OpenPath() override;

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

        void onInitialization() override;

    private:

        void drawButtonsPath();

        void drawItemsChild();

        void drawFileInput();

        void drawButtons();

        void updateFilesInCurrentPath();

        bool validateData(const FileData& data);

        bool isHidden(const FileData& data);

        bool hasProperExtension(const FileData& data);

        std::vector<FileData> m_files;

        std::filesystem::path m_currentPath;
        std::filesystem::path m_selected;

        OkCallback m_callback;

        Settings m_settings;

        bool m_currentOpenState;

        HG::Rendering::Base::Texture* m_directory;
        HG::Rendering::Base::Texture* m_file;
    };
}