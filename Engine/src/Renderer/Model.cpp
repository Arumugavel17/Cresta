#include "Model.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"

namespace Cresta
{
	Model::Model(const std::string& Path)
	{
		CRESTA_CORE_INFO("MAX SIZE: {0}", GL_MAX_TEXTURE_SIZE);
		LoadModel(Path);
		m_Shader = Shader::Create("assets/shaders/BindlessTextureShader.glsl");
	}

	void Model::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		bool condition = !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;

		CRESTA_ASSERT(condition,
			"ERROR::ASSIMP::{0}",
			importer.GetErrorString());

		m_Directory = path.substr(0, path.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(const aiNode* node, const aiScene* scene) 
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
			m_TexIndex++;
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			CRESTA_INFO("Processing Node {0}", i);
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
	{
		m_TextureCounter = 0;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		LoadMaterialTextures(material, aiTextureType_DIFFUSE);
		LoadMaterialTextures(material, aiTextureType_SPECULAR);
		
		for (int i = 0;i < mesh->mNumVertices;i++)
		{
			int16_t TextureIndex = 0;
			TextureIndex = TextureIndex << 5;
			TextureIndex |= m_TextureCounter & 0x1F;

			aiVector3D position = mesh->mVertices[i];
			aiVector3D texture = mesh->mTextureCoords[0][i];
			vertices.push_back({
				position.x,position.y,position.z,
				texture.x,texture.y,
				TextureIndex
				});
		}


		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		return Mesh(vertices, indices);
	}

	void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			m_TextureCounter++;
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++)
			{
				std::string textureName = m_TexturesLoaded[j]->GetPath().data();
				textureName = textureName.substr(textureName.find_last_of('/') + 1);
				if (std::strcmp(textureName.c_str(), str.C_Str()) == 0)
				{
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{
				// if texture hasn't been loaded already, load it
				std::string filename = std::string(str.C_Str());
				filename = m_Directory + '/' + filename;
				Ref<Texture2D> texture = Texture2D::Create(filename);
				texture->SetTextureType((TextureType)type);
				m_TexturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
				m_TextureHandles.push_back(texture->GetTextureHandle());
			}
		}
	}
	
	void Model::SetupVAO()
	{
		for (int i = 0;i < m_Meshes.size();i++)
		{
			m_VAOs.push_back(VertexArray::Create());
			int VerticesSize = m_Meshes[i].m_Vertices.size();
			void* Vertices = m_Meshes[i].m_Vertices.data();
			int IndicesSize = m_Meshes[i].m_Indices.size();
			uint32_t* Indices = m_Meshes[i].m_Indices.data();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(Vertices, sizeof(Vertex) * VerticesSize);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(Indices, IndicesSize);

			vertexBuffer->SetLayout({
					{ ShaderDataType::FVec3 , "aPos" },
					{ ShaderDataType::FVec2 , "aTexCoords" },
					{ ShaderDataType::Int , "aTexIndex" }
				});

			m_VAOs[i]->AddVertexBuffer(vertexBuffer);
			m_VAOs[i]->SetIndexBuffer(indexBuffer);
		}

		m_UBO = UniformBuffer::Create(sizeof(uint64_t) * m_TextureHandles.size(), 0, m_TextureHandles.data());
	}

	void Model::Draw()
	{
		m_UBO->Bind();
		for (int i = 0;i < m_Meshes.size();i++)
		{
			Renderer::Submit(m_Shader, m_VAOs[i], glm::translate(glm::mat4(1.0f),glm::vec3(1.0f)));
			Renderer::Submit(m_Shader, m_VAOs[i], glm::mat4(1.0f));
		}
	}
}