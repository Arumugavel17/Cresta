#pragma once
#include "Renderer/Shader.hpp"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace Cresta {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, int coune) override;

		void SetFloat(const std::string& name, float value) override;
		void SetFloatArray(const std::string& name, float* value, int coune) override;

		void SetVec2(const std::string& name, const glm::vec2& value) override;
		void SetVec3(const std::string& name, const glm::vec3& value) override;
		void SetVec4(const std::string& name, const glm::vec4& value) override;

		void SetMat2(const std::string& name, const glm::mat2& value);
		void SetMat3(const std::string& name, const glm::mat3& value);
		void SetMat4(const std::string& name, const glm::mat4& value) override;
	private:
		unsigned int m_ProgramID;
	};
}