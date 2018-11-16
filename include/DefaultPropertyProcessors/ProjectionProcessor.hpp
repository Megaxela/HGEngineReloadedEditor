#pragma once

// Editor
#include <AbstractPropertyProcessor.hpp>

namespace HG::Editor::PropertyProcessors
{
    /**
     * @brief Class, that performs projection property processing.
     */
    class ProjectionProcessor : public HG::Editor::AbstractPropertyProcessor
    {
    public:
        /**
         * @brief Method, that performs drawing of edit file with projection
         * selection field with all editing logic.
         * @param id Unique field id.
         * @param property Constant reference to property object.
         */
        void perform(const std::string &name, const HG::Core::Behaviour::Property &property) override;
    };
}