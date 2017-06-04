R"(
#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform vec4 LineColor;

void main()
{
    fragColor = LineColor;
}
)"
