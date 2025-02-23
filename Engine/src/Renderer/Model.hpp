#pragma once
#include "Crestaph.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/UniformBuffer.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stbi_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Cresta 
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texCoords;
		int index;
	};

	struct Mesh
	{
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		Mesh(std::vector<Vertex> vertices,
			std::vector<uint32_t> indices)
		{
			m_Vertices = vertices;
			m_Indices = indices;
		}
	};

	class Model 
	{
	public:
		std::vector<Ref<VertexArray>> m_VAOs;
		static std::unordered_map<std::string, std::pair<std::vector<Ref<VertexArray>>, Ref<UniformBuffer>>> s_ModelsLoaded;

	public:
		Model(int EntityID);
		Model(const std::string& Path);

		void DrawWireFrame(const glm::vec3& position);
		void Draw(const glm::vec3& position = glm::vec3(1.0f) , int m_EntityID = 0);
		void Draw(const glm::mat4& transform = glm::mat4(1.0f), int m_EntityID = 0);
		static Ref<Model> Create(const std::string& Path);
		
	private:
		
		void SetupVAO();
		void LoadModel(std::string Path);

		void ProcessNode(const aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);

		void LoadMaterialTextures(	aiMaterial* mat, aiTextureType type);
	private:
		
		std::string m_Path = "";

		uint16_t m_TexIndex = 0;
		uint8_t m_TextureCounter = 0;

		int m_NodeCount = 0;
		bool m_FBXModel = false;
		bool m_EmbeddedTexture = false;
		bool m_IsStatic = false; // To know if the model is static if yes then it can be batched else draw it individually.
		
		Ref<Shader> m_Shader;
		Ref<UniformBuffer> m_UniformBuffer;

		std::string m_Directory;
		std::vector<Mesh> m_Meshes;
		std::vector<Ref<Texture2D>> m_TexturesLoaded;
		std::vector<uint64_t> m_TextureHandles;
	};
}