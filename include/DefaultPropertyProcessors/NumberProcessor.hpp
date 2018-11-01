#pragma once

// C++ STD
#include <algorithm>

// Editor
#include <AbstractPropertyProcessor.hpp>

// ImGui
#include <imgui.h>

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
        void perform(std::size_t id, const std::string& name, const HG::Core::Behaviour::Property& property) override
        {
            auto key = name + "##" + std::to_string(id);

            auto value = property.getGetter<T>()();

            bool used = false;

            if constexpr (sizeof(value) < sizeof(int))
            {
                auto min = static_cast<int>(std::numeric_limits<T>::min());
                auto max = static_cast<int>(std::numeric_limits<T>::max());

                int proxy = value;

                used = ImGui::DragInt(key.c_str(), &proxy, 1.0f, min, max);

                value = static_cast<T>(proxy);
            }
            else
            {
                ImGuiDataType dataType = ImGuiDataType_S32;

                if constexpr (std::is_same<int32_t, T>::value)
                {
                    dataType = ImGuiDataType_S32;
                }
                else if constexpr (std::is_same<uint32_t, T>::value)
                {
                    dataType = ImGuiDataType_U32;
                }
                else if constexpr (std::is_same<int64_t, T>::value)
                {
                    dataType = ImGuiDataType_S64;
                }
                else if constexpr (std::is_same<uint64_t, T>::value)
                {
                    dataType = ImGuiDataType_U64;
                }

                used = ImGui::DragScalar(key.c_str(), dataType, &value, 1.0f);
            }

            if (used)
            {
                property.getSetter<T>()(value);
            }
        }
    };
}
