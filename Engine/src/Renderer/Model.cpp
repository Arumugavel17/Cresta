#include "Model.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"
#include "vector"

namespace Cresta
{
	std::unordered_map<uint64_t, Ref<Model>> Model::s_ModelsLoadedWithMap;

	Model::Model(const std::string& Path)
	{
		LoadModel(Path);

		std::cout << "BoneCounter: " << m_BoneCounter << "\n";

		for (auto& [name,BoneInfo] : m_BoneInfoMap)
		{
			std::cout << "Name: " << name << "\n";
			std::cout << "BoneInfo: { ID: " << BoneInfo.id << " }\n";
		}
		CRESTA_ASSERT("k");

		SetupVAO();
	}

	void Model::LoadModel(std::string path)
	{

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, 
			aiProcess_JoinIdenticalVertices | 
			aiProcess_GlobalScale | 
			aiProcess_FindDegenerates | 
			aiProcess_FixInfacingNormals |
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
			aiProcess_CalcTangentSpace |
			aiProcess_ConvertToLeftHanded);

		bool condition = !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;

		CRESTA_ASSERT(
			condition,
			"ERROR::ASSIMP::{0}",
			importer.GetErrorString());

		m_Directory = path.substr(0, path.find_last_of('\\'));

		if (scene && scene->HasTextures())
		{
			m_EmbeddedTexture = true;
			for (uint32_t t = 0; t < scene->mNumTextures; ++t)
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
				if (texture->mHeight > 0) 
				{
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
		if(scene)
			ProcessNode(scene->mRootNode, scene);
	}

	void Model::SetVertexBoneDataToDefault(Vertex& vertex)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			vertex.BoneIDs[i] = -1;
			vertex.Weights[i] = 0.0f;
		}
	}

	void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (vertex.BoneIDs[i] < 0)
			{
				vertex.Weights[i] = weight;
				vertex.BoneIDs[i] = boneID;
				break;
			}
		}
	}

	glm::mat4 ConvertAssimpMatrixToGLM(const aiMatrix4x4& aiMat) {
		return glm::mat4(
			aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
			aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
			aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
			aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
		);
	}

	void Model::ExctractBoneInfo(std::vector<Vertex>& vertices,const aiMesh* mesh, const aiScene* scene)
	{
		for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
			{
				BoneInfo newBoneInfo;
				newBoneInfo.id = m_BoneCounter;
				newBoneInfo.offset = ConvertAssimpMatrixToGLM(
					mesh->mBones[boneIndex]->mOffsetMatrix);
				m_BoneInfoMap[boneName] = newBoneInfo;
				boneID = m_BoneCounter;
				m_BoneCounter++;
			}
			else
			{
				boneID = m_BoneInfoMap[boneName].id;
			}
			assert(boneID != -1);
			auto weights = mesh->mBones[boneIndex]->mWeights;
			int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				assert(vertexId <= vertices.size());
				SetVertexBoneData(vertices[vertexId], boneID, weight);
			}
		}
	}

	void Model::ProcessNode(const aiNode* node, const aiScene* scene) 
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
			m_TexIndex++;
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			m_NodeCount++;
			CRESTA_INFO("Processing Node {0}", m_NodeCount);
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

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
			aiVector3D normals(1.0f);
			if (mesh->HasNormals())
			{
				normals = mesh->mNormals[i];
			}

			vertices.push_back({
					{ position.x, position.y, position.z },  // glm::vec3 position
					{normals.x,normals.y,normals.z},
					{ texture.x, texture.y },                // glm::vec2 texCoords
					TextureIndex,                             // index
					{ -1, -1, -1, -1 },                      // m_BoneIDs initialized properly
					{ 0.0f, 0.0f, 0.0f, 0.0f }              // m_Weights initialized properly
				});
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		ExctractBoneInfo(vertices, mesh, scene);

		return Mesh(vertices, indices);
	}

	void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
	{
		for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			m_TextureCounter++;
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (uint32_t j = 0; j < m_TexturesLoaded.size(); j++)
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
					{ ShaderDataType::FVec3, "aPos"			},
					{ ShaderDataType::FVec3, "aNormals"		},
					{ ShaderDataType::FVec2, "aTexCoords"   },
					{ ShaderDataType::Int,	 "aTexIndex"	},
					{ ShaderDataType::IVec4, "aBoneIDs"		},
					{ ShaderDataType::FVec4, "aWeights"		}
				});

			m_VAOs[i]->AddVertexBuffer(vertexBuffer);
			m_VAOs[i]->SetIndexBuffer(indexBuffer);
		}

		m_UniformBuffer = UniformBuffer::Create(sizeof(uint64_t) * m_TextureHandles.size(), 0, m_TextureHandles.data());
	}

	void Model::DrawWireFrame(Ref<Shader> shader,const glm::vec3& position)
	{
		if (!m_UniformBuffer || m_VAOs.size() <= 0 || m_IsStatic)
		{
			return;
		}
		m_UniformBuffer->Bind();

		for (int i = 0;i < m_VAOs.size();i++)
		{
			shader->Bind();
			shader->SetInt("u_EntityID", 0);
			Renderer::DrawGizmoIndexed(shader, m_VAOs[i], glm::translate(glm::mat4(1.0f), position));
		}
	}

	void Model::DrawWireFrame(Ref<Shader> shader, const glm::mat4& position)
	{
		if (!m_UniformBuffer || m_VAOs.size() <= 0 || m_IsStatic)
		{
			return;
		}
		m_UniformBuffer->Bind();

		for (int i = 0;i < m_VAOs.size();i++)
		{
			shader->Bind();
			shader->SetInt("u_EntityID", 0);
			Renderer::DrawGizmoIndexed(shader, m_VAOs[i], position);
		}
	}

	void Model::Draw(Ref<Shader> shader, const glm::vec3& position, int EntityID)
	{
		if (!m_UniformBuffer || m_VAOs.size() <= 0 || m_IsStatic)
		{
			return;
		}
		m_UniformBuffer->Bind();

		for (int i = 0;i < m_VAOs.size();i++)
		{
			shader->Bind();
			shader->SetInt("o_EntityID", EntityID);
			Renderer::DrawIndexed(shader, m_VAOs[i], glm::translate(glm::mat4(1.0f), position));
		}
	}

	void Model::Draw(Ref<Shader> shader, const glm::mat4& transform, int EntityID)
	{
		if (m_IsStatic)
		{
			return;
		}
		m_UniformBuffer->Bind();
		for (int i = 0;i < m_VAOs.size();i++)
		{
			shader->Bind();
			shader->SetInt("u_EntityID",EntityID);
			shader->SetVec3("u_LightPos", glm::vec3(1.0f));
			Renderer::DrawIndexed(shader, m_VAOs[i], transform);	
		}
	}

	Ref<Model> Model::Create(const std::string& Path, const uint64_t& ID)
	{
		if (s_ModelsLoadedWithMap.find(ID) == s_ModelsLoadedWithMap.end())
		{
			CRESTA_CORE_INFO("MAX SIZE: {0}", GL_MAX_TEXTURE_SIZE);
			s_ModelsLoadedWithMap[ID] = CreateRef<Model>(Path);
			return s_ModelsLoadedWithMap[ID];
		}
		return s_ModelsLoadedWithMap[ID];
	}

	Model::~Model()
	{
		std::cout << "\n";
	}
}