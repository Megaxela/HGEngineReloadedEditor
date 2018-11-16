#pragma once

// Editor
#include <AbstractPropertyProcessor.hpp>

namespace HG::Editor::PropertyProcessors
{
    /**
     * @brief Class, that performs color property processing.
     */
    class ColorProcessor : public HG::Editor::AbstractPropertyProcessor
    {
    public:

        void perform(const std::string &name, const HG::Core::Behaviour::Property &property) override;
    };
}
