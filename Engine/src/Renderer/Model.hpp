#pragma once
#include "Crestaph.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/UniformBuffer.hpp"
#include "ECS/UUID.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stbi_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MAX_BONE_INFLUENCE 4
#define MAX_BONES = 100;

namespace Cresta 
{
	namespace AssimpHelper
	{
		inline glm::mat4 ConvertAssimpMatrixToGLM(const aiMatrix4x4& aiMat) {
			return glm::mat4(
				aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
				aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
				aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
				aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
			);
		}

		inline glm::vec2 ConvertAssimpVec2ToGLM(const aiVector2D& vec)
		{
			return glm::vec2(vec.x, vec.y);
		}

		inline glm::vec3 ConvertAssimpVec3ToGLM(const aiVector3D& vec)
		{
			return glm::vec3(vec.x, vec.y, vec.z);
		}

		inline glm::quat ConvertAssimpQuatToGLM(const aiQuaternion& quat)
		{
			return glm::quat(quat.w, quat.x, quat.y, quat.z);
		}
	}

	struct BoneInfo
	{
		int id;
		glm::mat4 offset;

	};

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texCoords;
		int index;

		int BoneIDs[MAX_BONE_INFLUENCE];
		float Weights[MAX_BONE_INFLUENCE];
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
		static std::unordered_map<uint64_t, Ref<Model>> s_ModelsLoadedWithMap;

	public:
		Model(int EntityID);
		Model(const std::string& Path);
		~Model();
		auto& GetBoneInfoMap() { return m_BoneInfoMap; }
		int& GetBoneCount() { return m_BoneCounter; }

		void SetShader(Ref<Shader> shader);

		void DrawWireFrame(const glm::vec3& position);
		void DrawWireFrame(const glm::mat4& position);
		void Draw(const glm::vec3& position = glm::vec3(1.0f) , int m_EntityID = 0);
		void Draw(const glm::mat4& transform = glm::mat4(1.0f), int m_EntityID = 0);
		void MoveBone(glm::mat4 tansform, std::string boneIndex);
		static Ref<Model> Create(const std::string& Path, const uint64_t& ID);
		
	private:
		void SetVertexBoneDataToDefault(Vertex& vertex);
		void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
		void ExctractBoneInfo(std::vector<Vertex>& vertices,const aiMesh* mesh, const aiScene* scene);
		
		void SetupVAO();
		void LoadModel(std::string Path);

		void ProcessNode(const aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);

		void LoadMaterialTextures(	aiMaterial* mat, aiTextureType type);
	private:
		
		std::string m_Path = "";

		std::map<std::string, BoneInfo> m_BoneInfoMap; //
		int m_BoneCounter = 0;
		
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