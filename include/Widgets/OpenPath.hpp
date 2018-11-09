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

        /**
         * @brief Structure, that describes
         * dialog settings.
         */
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
        /**
         * @brief Method for drawing widget.
         */
        void onDraw() override;

        /**
         * @brief Method that performs icons
         * loading.
         */
        void onInitialization() override;

    private:

        /**
         * @brief Method for drawing path buttons.
         */
        void drawButtonsPath();

        /**
         * @brief Method for drawing items in directory.
         */
        void drawItemsChild();

        /**
         * @brief Method for drawing file input.
         */
        void drawFileInput();

        /**
         * @brief Method for drawing Ok/Cancel buttons.
         */
        void drawButtons();

        /**
         * @brief Method for updating files in selected path
         * to update `drawItemsChild` data.
         */
        void updateFilesInCurrentPath();

        /**
         * @brief Method for validating file data for display.
         */
        bool validateData(const FileData& data);

        /**
         * @brief Method for checking is some file hidden.
         * (Currently working only with dot files)
         * @param data
         * @return
         */
        bool isHidden(const FileData& data);

        /**
         * @brief Method for checking does
         * available extensions contains
         * file extension.
         * @param data
         * @return
         */
        bool hasProperExtension(const FileData& data);

        std::vector<FileData> m_files;

        std::filesystem::path m_currentPath;
        std::filesystem::path m_selected;

        std::string m_inputBuffer;

        OkCallback m_callback;

        Settings m_settings;

        bool m_currentOpenState;

        HG::Rendering::Base::Texture* m_directory;
        HG::Rendering::Base::Texture* m_file;
    };
}