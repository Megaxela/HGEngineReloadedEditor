// Editor
#include <Materials/ColorMaterial.hpp>

const char* HG::Editor::Materials::ColorMaterial::rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
}
#endif

#ifdef FragmentShader
uniform vec3 color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0f);
}
#endif
)";

void HG::Editor::Materials::ColorMaterial::setColor(const HG::Utils::Color &color)
{
    set("color", color.toRGBVector());
}
