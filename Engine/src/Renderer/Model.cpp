#include "Model.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"

namespace Cresta
{
	std::unordered_map<std::string, Ref<Model>> Model::s_ModelsLoaded;

	Model::Model(const std::string& Path)
	{
		CRESTA_CORE_INFO("MAX SIZE: {0}", GL_MAX_TEXTURE_SIZE);
		LoadModel(Path);
		m_Shader = Shader::Create("assets/shaders/BindlessTextureShader.glsl");
		SetupVAO();
	}

	void Model::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path,
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);

		bool condition = !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;

		CRESTA_ASSERT(
			condition,
			"ERROR::ASSIMP::{0}",
			importer.GetErrorString());

		m_Directory = path.substr(0, path.find_last_of('/'));
		if (path.substr(path.find_last_of('.'), path.length() - 1) == ".fbx")
		{
			m_FBXModel = true;
		}

		if (scene->HasTextures())
		{
			m_EmbeddedTexture = true;
			for (unsigned int t = 0; t < scene->mNumTextures; ++t)
			{
				aiTexture* texture = scene->mTextures[t];
				if (texture->mHeight == 0)
				{
					std::string format(texture->achFormatHint);
					std::string filename = texture->mFilename.C_Str();
					filename = m_Directory + "/" + filename.substr(filename.find_last_of('/') + 1,filename.length() - 1);

					std::ofstream outFile(filename, std::ios::binary);
					outFile.write(reinterpret_cast<const char*>(texture->pcData), texture->mWidth);
					outFile.close();

					CRESTA_INFO("Saved compressed texture as: {0}", filename);
					Ref<Texture2D> texture = Texture2D::Create(filename,false);

					if (filename.find("_diffuse") != -1)
					{
						texture->SetTextureType(TextureType::Diffuse);
					}
					else if (filename.find("_specular") != -1)
					{
						texture->SetTextureType(TextureType::Specular);
					}
					else if (filename.find("_normal") != -1)
					{
						texture->SetTextureType(TextureType::Normals);
					}

					m_TexIndex++;
					m_TextureCounter++;
					m_TexturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
					m_TextureHandles.push_back(texture->GetTextureHandle());
				}
				if (texture->mHeight > 0) {
					int width = texture->mWidth;
					int height = texture->mHeight;
					unsigned char* data = reinterpret_cast<unsigned char*>(texture->pcData);

					Ref<Texture2D> texture = Texture2D::Create( width, height, data);
					m_TexIndex++;
					m_TextureCounter++;
					m_TexturesLoaded.push_back(texture);
					m_TextureHandles.push_back(texture->GetTextureHandle());
				}
			}
		}
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
			m_NodeCount++;
			CRESTA_INFO("Processing Node {0}", m_NodeCount);
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (!m_EmbeddedTexture)
		{
			m_TextureCounter = 0;
			LoadMaterialTextures(material, aiTextureType_DIFFUSE);
			LoadMaterialTextures(material, aiTextureType_SPECULAR);
			LoadMaterialTextures(material, aiTextureType_BASE_COLOR);
			LoadMaterialTextures(material, aiTextureType_UNKNOWN);
		}
		
		for (int i = 0;i < mesh->mNumVertices;i++)
		{
			int16_t TextureIndex = 0;
			TextureIndex = TextureIndex << 5;
			TextureIndex |= m_TextureCounter & 0x1F;

			aiVector3D position = mesh->mVertices[i];
			aiVector3D texture = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f); 
			if (m_FBXModel)
			{
				vertices.push_back({
					{ position.x / 100,position.y / 100,position.z / 100 },
					{ texture.x,texture.y},
					TextureIndex
					});
			}
			else 
			{
				vertices.push_back({
					{ position.x,position.y,position.z },
					{ texture.x,texture.y },
					TextureIndex
					});
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
			m_VAOs[i]->Bind();
			 
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
		m_UniformBuffer = UniformBuffer::Create(sizeof(uint64_t) * m_TextureHandles.size(), 0, m_TextureHandles.data());
	}

	void Model::Draw(const glm::vec3& position)
	{
		if (m_IsStatic)
		{
			return;
		}
		m_UniformBuffer->Bind();
		for (int i = 0;i < m_Meshes.size();i++)
		{
			Renderer::DrawIndexed(m_Shader, m_VAOs[i], glm::translate(glm::mat4(1.0f), position));
		}
	}

	void Model::Draw(const glm::mat4& transform)
	{
		if (m_IsStatic)
		{
			return;
		}

		m_UniformBuffer->Bind();
		for (int i = 0;i < m_Meshes.size();i++)
		{
			Renderer::DrawIndexed(m_Shader, m_VAOs[i], transform);	
		}
	}

	Ref<Model> Model::Create(const std::string& Path)
	{
		Model::s_ModelsLoaded[Path] = CreateRef<Model>(Path);
		return Model::s_ModelsLoaded[Path];
	}
}