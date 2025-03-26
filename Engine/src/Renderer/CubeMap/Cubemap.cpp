#include "Cubemap.hpp"
#include "Renderer/Renderer.hpp"

#include <stbi_image/stb_image.h>

namespace Cresta
{
	CubeMap::CubeMap()
	{
        sm_VAO = VertexArray::Create();
		m_Shader = Shader::Create("assets\\shaders\\CubeMapShader.glsl");
        std::vector<float> skyboxVertices = {
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
                { ShaderDataType::FVec3, "aPos"}
            });

        Ref<IndexBuffer> IBO = IndexBuffer::Create(indices.data(), indices.size());
        sm_VAO->AddVertexBuffer(VBO);
        sm_VAO->SetIndexBuffer(IBO);

	}

	void CubeMap::SetTextures(std::vector<std::string> textures)
	{
        m_Textures = textures;
        stbi_set_flip_vertically_on_load(false);
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

        int width, height, nrChannels;

        for (unsigned int i = 0;i < m_Textures.size();i++)
        {
            unsigned char* data = stbi_load(m_Textures[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                    width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
                std::cout << "Cubemap succesfully loaded" << m_Textures[i] << std::endl;

            }
            else
            {
                std::cout << "Cubemap failed to load at path:" << m_Textures[i] << std::endl;
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

    void CubeMap::Draw()
    {
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);  // Change depth function to allow skybox rendering
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        Renderer::DrawIndexed(m_Shader,sm_VAO,glm::mat4(1.0f));
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS); // Restore default depth function
    }
}
