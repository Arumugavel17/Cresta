//type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = u_ProjectionView * u_Model * vec4(aPos,1.0);
}

//type fragment
#version 330 core

uniform int u_TexIndex;
in vec2 TexCoords;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Specular;
uniform vec4 u_Color;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int o_color;

void main()
{
    vec4 color;
    if(u_TexIndex == 0)
    {
        color = texture(u_Diffuse, TexCoords);
    }
    else if(u_TexIndex == 1)
    {
        color = texture(u_Specular, TexCoords);
    }
    FragColor = color;
    o_color = 50;
}