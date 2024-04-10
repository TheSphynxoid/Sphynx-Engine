#version 440 core
in vec2 TexCoord;
out vec4 color;
layout(binding = 0) uniform sampler2D Sprite;

void main()
{
    color = texture(Sprite, TexCoord);
}