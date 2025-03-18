//type vertex
#version 460 core
#extension GL_NV_gpu_shader5: enable
#extension GL_NV_uniform_buffer_std430_layout: enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in int16_t aTexIndex;


flat out int8_t TexCount;   
flat out int16_t TexIndex;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
    TexCount = (int8_t)aTexIndex & (int8_t)0x1F;  // Extract lower 5 bits
    TexIndex = aTexIndex >> 5;   // Extract upper 11 bits
    TexCoords = aTexCoords;
    gl_Position = u_ProjectionView * u_Model * vec4(aPos, 1.0);
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
uniform int o_EntityID;
in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int o_color;

void main()
{
    if(TexCount == 0)
    {
        FragColor = vec4(vec3(0.7),1.0);
        o_color = o_EntityID;
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
    o_color = o_EntityID;
}
