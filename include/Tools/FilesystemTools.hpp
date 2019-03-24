#pragma once

#include <filesystem>

namespace HG::Editor::FilesystemTools
{
    /**
     * @brief Method for finding unique path in provided directory for provided filename.
     * Example:
     * Path: some_directory
     * Name: Folder
     * Result: some_directory/Folder 2
     */
    std::filesystem::path findUniquePath(const std::filesystem::path& path, const std::string& name);
}