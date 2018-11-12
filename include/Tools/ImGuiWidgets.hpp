// ImGui
#include <imgui.h>

namespace HG::Rendering::Base
{
    class Texture;
}

namespace ImGui
{
    bool IconSelectable(const char* label, bool selected, int flags, const ImVec2& size_arg={0, 0}, HG::Rendering::Base::Texture* icon=nullptr);
}

ImVec2 operator+(const ImVec2& l, const ImVec2& r);
ImVec2 operator-(const ImVec2& l, const ImVec2& r);