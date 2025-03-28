#include "PrimitiveMeshes.hpp"

#include <yaml-cpp/yaml.h>
#include <imgui/imgui_internal.h>      // Internal functions (required for DockBuilder APIs)
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Cresta
{
    Ref<Model> Primitive::sm_CapsuleModel = nullptr;

	Ref<Shader> Primitive::sm_PrimitiveShader = nullptr;

    Ref<VertexArray> Primitive::sm_BoxVAO = nullptr;
    Ref<VertexArray> Primitive::sm_SphereVAO = nullptr;
    Ref<VertexArray> Primitive::sm_CapsuleVAO = nullptr;

    float Primitive::sm_SphereSectors = 8;
    float Primitive::sm_SphereStacks = 8;
    float Primitive::sm_SphereRadius = 1;

    float Primitive::sm_CapsuleSectors = 8;
    float Primitive::sm_CapsuleHeight = 5;
    float Primitive::sm_CapsuleRadius = 1;


    void Primitive::Init()
    {
        if (!sm_PrimitiveShader)
        {
            sm_PrimitiveShader = Shader::Create("assets/shaders/wireframeshader.glsl");
        }

        sm_BoxVAO = VertexArray::Create();
        sm_SphereVAO = VertexArray::Create();
        sm_CapsuleVAO = VertexArray::Create();

        {
            std::vector<float> boxVertices = {
                // Positions          // Normals
                // Front face
                 -1.0f, -1.0f,  1.0f,
                  1.0f, -1.0f,  1.0f,
                  1.0f,  1.0f,  1.0f,
                 -1.0f,  1.0f,  1.0f,

                 // Back face
                 -1.0f, -1.0f, -1.0f,
                 -1.0f,  1.0f, -1.0f,
                  1.0f,  1.0f, -1.0f,
                  1.0f, -1.0f, -1.0f,

                   // Left face
                 -1.0f, -1.0f, -1.0f,
                 -1.0f, -1.0f,  1.0f,
                 -1.0f,  1.0f,  1.0f,
                 -1.0f,  1.0f, -1.0f,

                   // Right face
                  1.0f, -1.0f, -1.0f,
                  1.0f,  1.0f, -1.0f,
                  1.0f,  1.0f,  1.0f,
                  1.0f, -1.0f,  1.0f,

                    // Top face
                 -1.0f,  1.0f, -1.0f,
                 -1.0f,  1.0f,  1.0f,
                  1.0f,  1.0f,  1.0f,
                  1.0f,  1.0f, -1.0f,

                     // Bottom face
                 -1.0f, -1.0f, -1.0f,
                  1.0f, -1.0f, -1.0f,
                  1.0f, -1.0f,  1.0f,
                 -1.0f, -1.0f,  1.0f
            };

            std::vector<uint32_t> boxIndices = {
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

            Ref<VertexBuffer> VBO = VertexBuffer::Create(boxVertices.data(), sizeof(float) * boxVertices.size());
            VBO->SetLayout({
                    { ShaderDataType::FVec3, "aPos"}
                });

            Ref<IndexBuffer> IBO = IndexBuffer::Create(boxIndices.data(), boxIndices.size());

            sm_BoxVAO->AddVertexBuffer(VBO);
            sm_BoxVAO->SetIndexBuffer(IBO);
        }

        {
            std::vector<float> sphereVertices;
            std::vector<uint32_t> sphereIndices;

            for (int i = 0; i <= sm_SphereStacks; ++i) 
            {
                float phi = IM_PI * i / sm_SphereStacks;
                for (int j = 0; j <= sm_SphereSectors; ++j) 
                {
                    float theta = 2 * IM_PI * j / sm_SphereSectors;
                    float x = sm_SphereRadius * sin(phi) * cos(theta);
                    float y = sm_SphereRadius * cos(phi);
                    float z = sm_SphereRadius * sin(phi) * sin(theta);

                    // Normal is simply the normalized vertex position
                    float nx = sin(phi) * cos(theta);
                    float ny = cos(phi);
                    float nz = sin(phi) * sin(theta);

                    sphereVertices.insert(sphereVertices.end(), { x, y, z, nx, ny, nz });
                }
            }

            for (int i = 0; i < sm_SphereStacks; ++i) 
            {
                for (int j = 0; j < sm_SphereSectors; ++j) 
                {
                    uint32_t first = i * (sm_SphereSectors + 1) + j;
                    uint32_t second = first + sm_SphereSectors + 1;
                    sphereIndices.insert(sphereIndices.end(), { first, second, first + 1 });
                    sphereIndices.insert(sphereIndices.end(), { second, second + 1, first + 1 });
                }
            }

            Ref<VertexBuffer> VBO = VertexBuffer::Create(sphereVertices.data(), sizeof(float) * sphereVertices.size());
            VBO->SetLayout({
                    { ShaderDataType::FVec3, "aPos"},
                    { ShaderDataType::FVec3, "aNormal"}
                });

            Ref<IndexBuffer> IBO = IndexBuffer::Create(sphereIndices.data(), sphereIndices.size());
            sm_SphereVAO->AddVertexBuffer(VBO);
            sm_SphereVAO->SetIndexBuffer(IBO);
        }

        {
            std::vector<float> capsuleVertices;
            std::vector<uint32_t> capsuleIndices;

            std::cout << "Vertices: \n";
            for (int j = 0;j < sm_CapsuleSectors;j++)
            {
                float phi = (2 * IM_PI * j) / sm_CapsuleSectors;
                float x = sm_CapsuleRadius * cos(phi);
                float y = 0.0f;
                float z = sm_CapsuleRadius * sin(phi);
                capsuleVertices.insert(capsuleVertices.end(),{x,y,z});

                //std::cout << "Vertex " << j * 2 << " : " << x << " " << y << " " << z << "\n";

                x = sm_CapsuleRadius * cos(phi);
                y = sm_CapsuleHeight;
                z = sm_CapsuleRadius * sin(phi);

                capsuleVertices.insert(capsuleVertices.end(), { x,y,z });
                //std::cout << "Vertex " << (j * 2) + 1 << " : " << x << " " << y << " " << z << "\n";
            }

            capsuleVertices.insert(capsuleVertices.end(), { capsuleVertices[0],capsuleVertices[1],capsuleVertices[2]});
            capsuleVertices.insert(capsuleVertices.end(), { capsuleVertices[3],capsuleVertices[4],capsuleVertices[5]});

            std::cout << "Indices: \n";
            for (int i = 0;i < sm_CapsuleSectors;i++)
            {
                uint32_t x = i * 2;
                capsuleIndices.insert(capsuleIndices.end(), { x, x + 2, x + 3 });
                //std::cout << "Indices " << x << " : " << x << " " << x + 2 << " " << x + 3 << "\n";
                capsuleIndices.insert(capsuleIndices.end(), { x + 3, x + 1, x });
                //std::cout << "Indices " << x + 1 << " : " << x + 3 << " " << x + 1 << " " << x << "\n";
            }

            Ref<VertexBuffer> VBO = VertexBuffer::Create(capsuleVertices.data(), sizeof(float) * capsuleVertices.size());
            VBO->SetLayout({
                    { ShaderDataType::FVec3, "aPos"}
                });

            Ref<IndexBuffer> IBO = IndexBuffer::Create(capsuleIndices.data(), capsuleIndices.size());
            sm_CapsuleVAO->AddVertexBuffer(VBO);
            sm_CapsuleVAO->SetIndexBuffer(IBO);
        }


        sm_CapsuleModel = Model::Create("assets\\models\\Cap.obj",0);
    }
	

	void Primitive::DrawCubeGuizmo(const glm::mat4& transform)
	{
        Renderer::DrawGizmoIndexed(sm_PrimitiveShader, sm_BoxVAO, transform);
	}

	void Primitive::PrimitiveCube(Ref<Shader> shader,const glm::mat4& transform)
	{
        Renderer::DrawIndexed(sm_PrimitiveShader, sm_BoxVAO, transform);
    }

    void Primitive::DrawSphereGuizmo(const glm::vec3& position, const glm::quat& rotation, float radius)
    {
        glm::mat4 temp = glm::translate(glm::mat4(1.0f), position)
            * glm::toMat4(rotation)
            * glm::scale(glm::mat4(1.0f), glm::vec3(radius));

        Renderer::DrawGizmoIndexed(sm_PrimitiveShader, sm_SphereVAO, temp);
    }

    void Primitive::PrimitiveSphere(Ref<Shader> shader, float radius)
    {
        Renderer::DrawIndexed(shader, sm_SphereVAO, glm::scale(glm::mat4(1.0f), glm::vec3(radius)));
    }

    void Primitive::DrawCapsuleGuizmo(const glm::mat4& transform)
    {
        sm_CapsuleModel->DrawWireFrame(sm_PrimitiveShader,transform);
    }

    void Primitive::PrimitiveCapsule(Ref<Shader> shader, const glm::mat4& transform)
    {
        sm_CapsuleModel->Draw(shader, transform);
    }
}