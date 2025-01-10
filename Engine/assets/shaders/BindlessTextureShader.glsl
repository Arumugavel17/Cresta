//type vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in float aTexIndex;

flat out int TexIndex;
out vec2 TexCoords;
uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
    TexIndex = int(aTexIndex);
    TexCoords = aTexCoords;
    gl_Position = u_ProjectionView * u_Model * vec4(aPos,1.0);
}

//type fragment
#version 420 core
#extension GL_NV_uniform_buffer_std430_layout: enable
#extension GL_NV_gpu_shader5: enable
#extension GL_ARB_bindless_texture: require

layout(std430, binding = 0) uniform TextureHandles {
    uvec2 textureHandles[1500];  // Array of texture handles
};

flat in int TexIndex;
in vec2 TexCoords;
out vec4 FragColor;

void main()
{
    uint64_t textureHandle = textureHandles[TexIndex].x;
    sampler2D tex = sampler2D(textureHandle);
    FragColor = texture(tex,TexCoords);
}
