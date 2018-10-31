#pragma once

// Editor
#include <AbstractPropertyProcessor.hpp>

namespace HG::Editor::PropertyProcessors
{
    /**
     * @brief Class, that performs any of number processing.
     * @tparam T Actual type.
     */
    template<typename T>
    class NumberProcessor : public HG::Editor::AbstractPropertyProcessor
    {
    public:

        /**
         * @brief Method, that performs drawing of number edit field
         * @param id
         * @param property
         */
        void perform(std::size_t id, const HG::Core::Behaviour::Property& property) override
        {
            auto key = "##" + property.name() + std::to_string(id);

            auto value = static_cast<int>(property.getGetter<T>()());

            int min = std::numeric_limits<int>::min();
            int max = std::numeric_limits<int>::max();

            // Min value
            if constexpr (std::numeric_limits<T>::min() >= std::numeric_limits<int>::min())
            {
                min = static_cast<int>(std::numeric_limits<T>::min());
            }

            if constexpr (std::numeric_limits<T>::max() <= std::numeric_limits<int>::max())
            {
                max = static_cast<int>(std::numeric_limits<T>::max());
            }

            if (ImGui::DragInt(key.c_str(), &value, 1.0f, min, max))
            {
                property.getSetter<T>()(static_cast<T>(value));
            }
        }
    };
}
