#pragma once

// Editor
#include <AbstractPropertyProcessor.hpp>

namespace HG::Editor::PropertyProcessors
{
    /**
     * @brief Class, that performs float property processing.
     */
    class FloatProcessor : public HG::Editor::AbstractPropertyProcessor
    {
    public:

        /**
         * @brief Method, that performs drawing of float edit field
         * with all editing logic.
         * @param id Unique field id.
         * @param property Constant reference to property object.
         */
        void perform(std::size_t id, const HG::Core::Behaviour::Property &property) override;
    };
}