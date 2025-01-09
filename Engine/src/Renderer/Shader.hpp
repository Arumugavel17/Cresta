#pragma once

#include "Crestaph.hpp"
#include "Renderer/Texture.hpp"

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Cresta {

	class Shader
	{
	public:

		Shader(const std::string& filepath);
		Shader(const std::string& filepath,const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetTexture(Ref<Texture2D> texture) = 0;
		virtual void SetTexture(Ref<Texture2D>* texture,uint32_t size) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;

		virtual void SetIntArray(const std::string& name, int* values, int coune) = 0;
		virtual void SetFloatArray(const std::string& name, float* value, int coune) = 0;
		
		virtual void SetVec2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetVec4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		const std::string& GetFilePath() const { return m_filepath; };

		std::string ReadFile(const std::string& filepath);
		Ref<std::unordered_map<GLenum, std::string>> PreProcess(const std::string& source);
		static GLenum ShaderTypeFromString(const std::string& type);

		static Ref<Shader> Create(const std::string filepath);
		static Ref<Shader> Create(const std::string filepath, const std::string& vertexSrc, const std::string& fragmentSrc);
	protected:
		std::string m_filepath;
		Ref<std::unordered_map<GLenum, std::string>> m_ShaderSrc;
	};

	class ShaderLibrary
	{
	public:
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Get(const std::string& filepath);

		void Add(const Ref<Shader>& shader);

		bool Exists(const std::string& filepath) const;
	
	private:
		//filepath and Shader Object
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}
