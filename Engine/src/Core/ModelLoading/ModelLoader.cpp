#include "ModelLoader.hpp"

namespace Cresta
{
	ModelLoader::ModelLoader(const std::string& Path)
	{
		LoadModel(Path);
		SetupVAO();
	}

	void ModelLoader::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		bool condition = !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;
		CRESTA_ASSERT(condition,
			"ERROR::ASSIMP::{0}",
			importer.GetErrorString());

		CRESTA_CORE_INFO("Model Load Started");

		// retrieve the directory path of the filepath
		m_Directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		ProcessNode(scene->mRootNode, scene);
		CRESTA_CORE_INFO("Model loaded");
	}

	void ModelLoader::ProcessNode(const aiNode* node, const aiScene* scene) 
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			CRESTA_CORE_INFO("Loading Mesh Number: {0}",i);

			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}

		// after we've processed all of the m_Meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			CRESTA_CORE_INFO("Processing Node {0}",i);
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh ModelLoader::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<Ref<Texture2D>> textures;

		for (int i = 0;i < mesh->mNumVertices;i++) 
		{	
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);

			// normals
			if (mesh->HasNormals())
			{
				vertices.push_back(mesh->mNormals[i].x);
				vertices.push_back(mesh->mNormals[i].y);
				vertices.push_back(mesh->mNormals[i].z);
			}

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vertices.push_back(mesh->mTextureCoords[0][i].x);
				vertices.push_back(mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
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

	std::vector<Ref<Texture2D>> ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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
				if (std::strcmp(m_TexturesLoaded[j]->GetPath().data(), str.C_Str()) == 0)
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

	void ModelLoader::SetupVAO()
	{
		std::vector<float> FloatArray;
		std::vector<uint32_t> IntArray;

		for (int i = 0;i < m_Meshes.size();i++)
		{
			FloatArray.insert(FloatArray.end(), m_Meshes[i].m_Vertices.begin(), m_Meshes[i].m_Vertices.end());
			IntArray.insert(IntArray.end(), m_Meshes[i].m_Indices.begin(), m_Meshes[i].m_Indices.end());
		}

		float* vertices = FloatArray.data();
		uint32_t* indices = IntArray.data();

		Ref<VertexBuffer> VBO = VertexBuffer::Create(vertices, sizeof(float) * FloatArray.size());
		Ref<IndexBuffer> IBO = IndexBuffer::Create(indices, IntArray.size());

		m_VAO = VertexArray::Create();
		m_VAO->AddVertexBuffer(VBO);
		m_VAO->SetIndexBuffer(IBO);
	}
}