#include "Model.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"

namespace Cresta
{
	Model::Model(const std::string& Path)
	{
		CRESTA_CORE_INFO("MAX SIZE: {0}", GL_MAX_TEXTURE_SIZE);
		LoadModel(Path);
		m_Shader = Shader::Create("assets/shaders/ModelShader.glsl");
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
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			CRESTA_INFO("Processing Node {0}", i);
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<Ref<Texture2D>> textures;

		for (int i = 0;i < mesh->mNumVertices;i++)
		{
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);
			
			if (mesh->mTextureCoords[0])
			{
				vertices.push_back(mesh->mTextureCoords[0][i].x);
				vertices.push_back(mesh->mTextureCoords[0][i].y);
			}
		}


		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Ref<Texture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Ref<Texture2D>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		return Mesh(vertices, indices, textures);
	}

	std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Ref<Texture2D>> textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
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
					textures.push_back(m_TexturesLoaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{
				// if texture hasn't been loaded already, load it
				Ref<Texture2D> texture;
				std::string filename = std::string(str.C_Str());
				filename = m_Directory + '/' + filename;
				texture = Texture2D::Create(filename);
				texture->SetTextureType((TextureType)type);
				textures.push_back(texture);
				m_TexturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			}
		}
		return textures;
	}
	
	void Model::SetupVAO()
	{		
		for (int i = 0;i < m_Meshes.size();i++)
		{
			m_VAOs.push_back(VertexArray::Create());
			int VerticesSize = m_Meshes[i].m_Vertices.size();
			float* Vertices = m_Meshes[i].m_Vertices.data();
			int IndicesSize = m_Meshes[i].m_Indices.size();
			uint32_t* Indices = m_Meshes[i].m_Indices.data();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(Vertices, sizeof(float) * VerticesSize);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(Indices, IndicesSize);

			vertexBuffer->SetLayout({
					{ ShaderDataType::FVec3 , "aPos" },
					{ ShaderDataType::FVec2 , "aTexCoords" }
				});

			m_VAOs[i]->AddVertexBuffer(vertexBuffer);
			m_VAOs[i]->SetIndexBuffer(indexBuffer);
		}
	}

	void Model::Draw()
	{
		for (int i = 0;i < m_Meshes.size();i++)
		{
			m_Shader->Bind();
			m_Shader->SetVec4("u_Color", glm::vec4(glm::vec3(0.5), 1.0f));
			for (uint32_t k = 0; k < m_Meshes[k].m_Textures.size(); k++)
			{
				m_Shader->SetTexture(m_Meshes[i].m_Textures.data(), m_Meshes[i].m_Textures.size());
			}
			m_Shader->Unbind();
			Renderer::Submit(m_Shader, m_VAOs[i], glm::mat4(1.0f));
		}
	}
}