R"(
#version 330 core

layout (location = 0) in vec2 Vertex;
layout (location = 1) in vec2 TexCoord;

out vec2 texCoord;

uniform mat4 MVP;
uniform vec4 TextOffset;

void main()
{
    texCoord    = TexCoord;
    gl_Position = (MVP * vec4(Vertex, 0.0f, 1.0f)) + TextOffset;
}
)"
