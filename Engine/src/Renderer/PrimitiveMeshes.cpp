#include "PrimitiveMeshes.hpp"
#include "PrimitiveMeshes.hpp"

#include <yaml-cpp/yaml.h>

namespace Cresta
{
	Ref<Model> Primitive::sm_CubeModel = nullptr;
	Ref<Shader> Primitive::sm_PrimitiveShader = nullptr;
    Ref<VertexArray> Primitive::sm_VAO = nullptr;

	void Primitive::Init()
	{
        sm_VAO = VertexArray::Create();
		if (!sm_PrimitiveShader)
		{
			sm_PrimitiveShader = Shader::Create("assets/shaders/wireframeshader.glsl");
		}

		std::vector<float> skyboxVertices = {
            // Positions          // Normals
            // Front face
            -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,

            // Back face
            -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,

            // Left face
            -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,

            // Right face
             0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,

            // Top face
            -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,

            // Bottom face
            -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f
		};

        std::vector<uint32_t> indices = {
            // Front face
            0, 1, 2,  2, 3, 0,
            // Back face
            4, 5, 6,  6, 7, 4,
            // Left face
            8, 9, 10,  10, 11, 8,
            // Right face
            12, 13, 14,  14, 15, 12,
            // Top face
            16, 17, 18,  18, 19, 16,
            // Bottom face
            20, 21, 22,  22, 23, 20
        };


        Ref<VertexBuffer> VBO = VertexBuffer::Create(skyboxVertices.data(), sizeof(float) * skyboxVertices.size());
        VBO->SetLayout({
                { ShaderDataType::FVec3, "aPos"},
                { ShaderDataType::FVec3, "aNormal"}
            });

        Ref<IndexBuffer> IBO = IndexBuffer::Create(indices.data(), indices.size());
        sm_VAO->AddVertexBuffer(VBO);
        sm_VAO->SetIndexBuffer(IBO);
	}

	void Primitive::DrawCubeGuizmo(const glm::mat4& transform)
	{
        Renderer::DrawGizmoIndexed(sm_PrimitiveShader, sm_VAO, transform);
	}

	void Primitive::PrimitiveCube(Ref<Shader> shader,const glm::mat4& transform)
	{
        Renderer::DrawIndexed(sm_PrimitiveShader, sm_VAO, transform);
    }
}