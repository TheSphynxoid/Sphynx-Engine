#version 440 core
in vec2 TexCoords;
out vec4 color;
layout(binding = 0) uniform sampler2D Front;
layout(binding = 1) uniform sampler2D Back;
void main()
{
    if(gl_FrontFacing){
        color = texture(Front, TexCoords);
    }else{
        color = texture(Back, TexCoords);
    }
}