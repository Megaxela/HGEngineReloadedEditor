#include <Tools/FilesystemTools.hpp>

std::filesystem::path HG::Editor::FilesystemTools::findUniquePath(const std::filesystem::path &path, const std::string &name)
{
    for (std::size_t i = 0; i < std::numeric_limits<std::size_t>::max(); ++i)
    {
        auto presumablePath = path / (name + " " + std::to_string(i));

        if (!std::filesystem::exists(presumablePath))
        {
            return presumablePath;
        }
    }

    throw std::runtime_error("Can't find presumable path for \"" + name + "\" in \"" + path.string() + "\"");
}
