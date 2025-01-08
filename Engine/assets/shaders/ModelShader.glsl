//type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
    gl_Position = u_ProjectionView * u_Model * vec4(aPos,1.0);
}

//type fragment
#version 330 core

uniform vec4 u_Color;
out vec4 FragColor;

void main()
{
    FragColor = u_Color;
}