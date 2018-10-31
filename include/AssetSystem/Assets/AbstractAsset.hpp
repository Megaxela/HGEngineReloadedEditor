#pragma once

// C++ STL
#include <filesystem>

namespace HG::Editor::AssetSystem::Assets
{
    /**
     * @brief Class, that describes abstract engine
     * asset.
     */
    class AbstractAsset
    {
    public:

        /**
         * @brief Constructor.
         * @param path Path.
         */
        explicit AbstractAsset(std::filesystem::path path);

        /**
         * @brief Virtual destructor.
         */
        virtual ~AbstractAsset();

        /**
         * @brief Virtual method for getting asset name.
         * It will return filename without extension.
         * @return Asset name.
         */
        virtual std::string name() const;

        /**
         * @brief Method for getting asset path.
         * @return Asset path.
         */
        std::filesystem::path path() const;

    protected:

        /**
         * @brief Method, that called on loading stage.
         */
        virtual void onLoad();

    private:

    };
}


