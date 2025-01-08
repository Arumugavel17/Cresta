#include "Model.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"

namespace Cresta
{
	Model::Model(const std::string& Path)
	{
		CRESTA_CORE_INFO("MAX SIZE: {0}", GL_MAX_TEXTURE_SIZE);
		m_Shader = Shader::Create("assets/shaders/ModelShader.glsl");
		LoadModel(Path);
		SetupVAO();
	}

	void Model::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		bool condition = !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;

		CRESTA_ASSERT(condition,
			"ERROR::ASSIMP::{0}",
			importer.GetErrorString());

		// retrieve the directory path of the filepath
		m_Directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(const aiNode* node, const aiScene* scene) 
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).

			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}

		// after we've processed all of the m_Meshes (if any) we then recursively process each of the children nodes
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

		return Mesh(vertices, indices);
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
				textureName = textureName.substr(textureName.find_last_of('/')+1);
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
				textures.push_back(texture);
				m_TexturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			}
		}
		return textures;
	}

	void Model::SetupVAO()
	{
		m_VertexArray = VertexArray::Create();
		int IndicesSize = 0;
		std::vector<unsigned int> Indices;
		for (int i = 0;i < m_Meshes.size();i++)
		{	
			IndicesSize += m_Meshes[i].m_Indices.size();
			if (IndicesSize >= 1600 && Indices.size() > 0)
			{
				Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(Indices.data(), IndicesSize - m_Meshes[i].m_Indices.size());
				m_VertexArray->SetIndexBuffer(indexBuffer);
				m_Shader->Bind();
				m_Shader->SetVec4("u_Color", glm::vec4(1.0f));
				m_Shader->Unbind();
				Renderer::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));
				m_VertexArray = VertexArray::Create();
				Indices.clear();
				IndicesSize = m_Meshes[i].m_Indices.size();
			}

			Indices.insert(Indices.end(),m_Meshes[i].m_Indices.begin(), m_Meshes[i].m_Indices.end());
			int VerticesSize = m_Meshes[i].m_Vertices.size();
			float* Vertices = m_Meshes[i].m_Vertices.data();

			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(Vertices, sizeof(float) * VerticesSize);

			vertexBuffer->SetLayout({
					{ ShaderDataType::FVec3 , "aPos" },
					{ ShaderDataType::FVec2 , "aTexCoords" }/*,
					{ ShaderDataType::Int , "aTexIndex" }*/
				});
			
			m_VertexArray->AddVertexBuffer(vertexBuffer);
		}		
		m_Shader->Bind();
		m_Shader->SetVec4("u_Color", glm::vec4( 0.0,1.0,0.0,1.0f));
		m_Shader->Unbind();
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(Indices.data(), IndicesSize);
		m_VertexArray->SetIndexBuffer(indexBuffer);
		Renderer::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));
	}
}