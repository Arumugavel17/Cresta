//type vertex
#version 460 core
#extension GL_NV_gpu_shader5: enable
#extension GL_NV_uniform_buffer_std430_layout: enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in int16_t aTexIndex;

flat out int8_t TexCount;   
flat out int16_t TexIndex;

out vec3 Fragpos;
out vec3 Normals;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
    TexCount = (int8_t)aTexIndex & (int8_t)0x1F;  // Extract lower 5 bits
    TexIndex = aTexIndex >> 5;   // Extract upper 11 bits

    Fragpos = vec3(u_Model*vec4(aPos,1.0f));
    TexCoords = aTexCoords;
    Normals = normalize(aNormals);
    gl_Position = u_ProjectionView * u_Model * vec4(aPos,1.0f);
}

//type fragment
#version 460 core
#extension GL_NV_gpu_shader5: enable
#extension GL_NV_uniform_buffer_std430_layout: enable
#extension GL_EXT_shader_explicit_arithmetic_types : enable
#extension GL_ARB_bindless_texture: require

layout(std430, binding = 0) uniform TextureHandles 
{
    uint64_t textureHandles[5];  // Array of 64-bit texture handles
};

sampler2D Diffuse;
sampler2D Specular;

flat in int8_t TexCount;
flat in int16_t TexIndex;

uniform int u_EntityID;

uniform vec3 u_LightPos;

in vec3 Fragpos;
in vec3 Normals;
in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int o_EntityID;

void main()
{
    if(TexCount == 0)
    {
        vec4 color = vec4(vec3(0.7),1.0); // Example: accumulate colors    

        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * vec3(1.0f);
        
        vec3 lightDir = u_LightPos - Fragpos;
        lightDir = normalize(lightDir);    
        float diff = max(dot(Normals, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0f);

        FragColor = vec4((ambient + diffuse),1.0f) * color;
        o_EntityID = u_EntityID;
        return;
    }
    if(TexCount >= int8_t(0))
    {
        uint64_t textureHandle = textureHandles[TexIndex];
        Diffuse = sampler2D(textureHandle);
    }
    if(TexCount >= int8_t(1))
    {
        uint64_t textureHandle = textureHandles[TexIndex + int16_t(1)];
        Specular = sampler2D(textureHandle);
    }
    vec4 color = texture(Diffuse, TexCoords); // Example: accumulate colors    

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0f);
    
    vec3 lightDir = u_LightPos - Fragpos;
    lightDir = normalize(lightDir);    
    float diff = max(dot(Normals, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0f);

    FragColor = vec4((ambient + diffuse),1.0f) * color;
    o_EntityID = u_EntityID;
}
