#pragma once

// Editor
#include <AbstractPropertyProcessor.hpp>

namespace HG::Editor::PropertyProcessors
{
    /**
     * @brief Class, that performs gameobject processing.
     */
    class GameObjectProcessor : public HG::Editor::AbstractPropertyProcessor
    {
    public:
        void perform(std::size_t id, const std::string &name, const HG::Core::Behaviour::Property &property) override;

    public:


    };
}


