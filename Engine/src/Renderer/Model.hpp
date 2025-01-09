#pragma once
#include "Crestaph.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stbi_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Cresta 
{
	struct Mesh
	{
		std::vector<float> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<Ref<Texture2D>> m_Textures;

		Mesh(std::vector<float> vertices,
			std::vector<uint32_t> indices,
			std::vector<Ref<Texture2D>> textures)
		{
			m_Vertices = vertices;
			m_Indices = indices;
			m_Textures = textures;
		}
	};

	class Model 
	{
	public:
		Model(const std::string& Path);
		void Draw();
		void SetupVAO();
	private:
		void LoadModel(std::string Path);

		void ProcessNode(const aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);

		std::vector<Ref<Texture2D>> LoadMaterialTextures(	aiMaterial* mat,
													aiTextureType type, 
													std::string typeName);
	private:
		std::vector<Ref<VertexArray>> m_VAOs;
		Ref<Shader> m_Shader;
		std::string m_Directory;
		std::vector<Mesh> m_Meshes;
		std::vector<Ref<Texture2D>> m_TexturesLoaded;
	public:
		Ref<VertexArray> m_VertexArray;
	};
}