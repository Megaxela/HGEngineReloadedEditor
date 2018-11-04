#pragma once

// C++ STL
#include <memory>
#include <filesystem>

namespace HG::Editor::AssetSystem::Assets
{
    class AbstractAsset;
}

namespace HG::Editor::Fabrics
{
    /**
     * @brief Class that describes fabric for
     * project assets.
     */
    class AssetsFabric
    {
    public:

        using AssetPtr = std::shared_ptr<HG::Editor::AssetSystem::Assets::AbstractAsset>;

        /**
         *
         * @param path
         * @return
         */
        AssetPtr create(std::filesystem::path path);

    private:



    };
}


