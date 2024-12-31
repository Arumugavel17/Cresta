//type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}

//type fragment
#version 330 core
out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
}