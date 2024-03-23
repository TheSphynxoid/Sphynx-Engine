#version 440 core
in vec2 TexCoord;
out vec4 color;
layout(binding = 0) uniform sampler2D Front;
layout(binding = 1) uniform sampler2D Back;
void main()
{
    if(gl_FrontFacing){
        color = texture(Front, TexCoord);
    }else{
        color = texture(Back, TexCoord);
    }
}