//type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    TexCoords = aPos;
    gl_Position = u_Projection * mat4(mat3(u_View)) * vec4(aPos, 1.0);
}  

//type fragment
#version 330 core

in vec3 TexCoords;
layout (location = 0) out vec4 FragColor;
uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}