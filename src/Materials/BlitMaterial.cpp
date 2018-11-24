// Editor
#include <Materials/BlitMaterial.hpp>

const char* HG::Editor::Materials::BlitMaterial::rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec2 inPosition;

uniform

void main()
{
    gl_Position = vec3(inPosition * , 0.0);
}
)";

void HG::Editor::Materials::BlitMaterial::setTexture(HG::Rendering::Base::Texture *texture)
{

}