#pragma once
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


namespace Cresta {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& filepath, const const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetTexture(Ref<Texture2D> texture) override;
		virtual void SetTexture(Ref<Texture2D>* texture, uint32_t size) override;
		virtual void SetIntArray(const std::string& name, int* values, int count) override;
		
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloatArray(const std::string& name, float* value, int count) override;
		
		virtual void SetVec2(const std::string& name, const glm::vec2& value) override;
		virtual void SetVec3(const std::string& name, const glm::vec3& value) override;
		virtual void SetVec4(const std::string& name, const glm::vec4& value) override;
		
		void SetMat2(const std::string& name, const glm::mat2& value);
		void SetMat3(const std::string& name, const glm::mat3& value);
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
	private:
		unsigned int m_ProgramID;
		void CreateProgram();
		unsigned int CompileShader(GLenum type, const char* shaderSrc);
	};
}