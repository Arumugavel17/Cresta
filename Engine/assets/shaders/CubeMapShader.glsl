//type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
    TexCoords = aPos;
    gl_Position = u_ProjectionView * u_Model * vec4(aPos, 1.0);
}  

//type fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}