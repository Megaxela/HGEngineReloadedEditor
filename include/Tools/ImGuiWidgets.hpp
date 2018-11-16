// ImGui
#include <imgui.h>

namespace HG::Rendering::Base
{
    class Texture;
}

namespace ImGui
{
    bool IconSelectable(const char* label, bool selected, int flags, const ImVec2& size_arg={0, 0}, HG::Rendering::Base::Texture* icon=nullptr);

    /**
     * @brief Class, that performs pushing and popping ImGui
     * ID with RAII.
     */
    class IDGuard
    {
    public:
        explicit IDGuard(int id);

        explicit IDGuard(void* id);

        explicit IDGuard(const char* id);

        explicit IDGuard(const char* begin_id, const char* end_id);

        ~IDGuard();
    };

    /**
     * @brief Class, that performs pushing and popping ImGui
     * disabled state and style with RAII.
     */
    class DisabledGuard
    {
    public:
        DisabledGuard();

        ~DisabledGuard();
    };
}

ImVec2 operator+(const ImVec2& l, const ImVec2& r);
ImVec2 operator-(const ImVec2& l, const ImVec2& r);