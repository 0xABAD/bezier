R"(
#version 330 core

uniform sampler2D TextTexture;
uniform vec4      TextColor;

in vec2 texCoord;

out vec4 Fragment;

void main()
{
    vec4 alpha = texture(TextTexture, texCoord);

    Fragment = vec4(TextColor.rgb, alpha.r);
}
)"

