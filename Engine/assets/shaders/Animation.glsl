//type vertex
#version 460 core
#extension GL_NV_gpu_shader5: enable
#extension GL_NV_uniform_buffer_std430_layout: enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in int16_t aTexIndex;
layout (location = 4) in ivec4 boneIds; 
layout (location = 5) in vec4 weights;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

flat out int8_t TexCount;   
flat out int16_t TexIndex;
out vec3 Normals;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * weights[i];
    }	

    TexCount = (int8_t)aTexIndex & (int8_t)0x1F;  // Extract lower 5 bits
    TexIndex = aTexIndex >> 5;   // Extract upper 11 bits
    TexCoords = aTexCoords;
    Normals = aNormals;
    gl_Position = u_ProjectionView * u_Model * totalPosition;
}



//type fragment
#version 460 core
#extension GL_NV_gpu_shader5: enable
#extension GL_NV_uniform_buffer_std430_layout: enable
#extension GL_EXT_shader_explicit_arithmetic_types : enable
#extension GL_ARB_bindless_texture: require

layout(std430, binding = 0) uniform TextureHandles {
    uint64_t textureHandles[5];  // Array of 64-bit texture handles
};

sampler2D Diffuse;
sampler2D Specular;

flat in int8_t TexCount;
flat in int16_t TexIndex;
uniform vec3 u_LightPos;
uniform int u_EntityID;

in vec3 Normals;
in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int o_EntityID;

void main()
{
    if(TexCount == 0)
    {
        FragColor = vec4(vec3(0.7),1.0);
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

    FragColor = color;
    o_EntityID = u_EntityID;
}
