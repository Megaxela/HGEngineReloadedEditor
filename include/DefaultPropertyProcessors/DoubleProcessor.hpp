#pragma once

// Editor
#include <AbstractPropertyProcessor.hpp>

namespace HG::Editor::PropertyProcessors
{
    /**
     * @brief Class, that performs double property processing.
     */
    class DoubleProcessor : public HG::Editor::AbstractPropertyProcessor
    {
    public:

        /**
         * @brief Method, that performs drawing of double edit field
         * with all editing logic.
         * @param id Unique field id.
         * @param property Constant reference to property object.
         */
        void perform(const std::string &name, const HG::Core::Behaviour::Property &property) override;
    };
}