//type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 u_ProjectionView;
uniform mat4 u_Model;

out vec2 TexCoord;

void main()
{
    gl_Position = u_ProjectionView * u_Model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

//type fragment
#version 330 core

in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int o_color;

uniform sampler2D u_Texture;
uniform int u_TextureSet = 0;
uniform vec4 u_Color = vec4(1.0);
uniform float u_Mixfactor = 0.0;
    
void main()
{
    if(u_TextureSet == 0)
    {
        FragColor = u_Color;
    }
    else if(u_TextureSet == 1)
    {
        vec4 frag = texture(u_Texture, TexCoord);
        FragColor = vec4(mix(u_Color,frag,u_Mixfactor));
    }
    o_color = 60;
}