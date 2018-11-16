#pragma once

// HG::Core
#include <HG/Core/Behaviour.hpp>

namespace HG::Editor
{
    /**
     * @brief Class, that describes abstract
     * property type processor.
     */
    class AbstractPropertyProcessor
    {
    public:

        /**
         * @brief Method, that performs actual processing function.
         * @param id Unique property id.
         * @param property Constant reference to property.
         */
        virtual void perform(const std::string &name, const HG::Core::Behaviour::Property &property) = 0;
    };
}