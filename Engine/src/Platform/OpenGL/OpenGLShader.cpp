#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Cresta {
	
	OpenGLShader::OpenGLShader(const std::string& filepath, 
							   const const std::string& vertexSrc, 
							   const std::string& fragmentSrc) 
		: Shader( filepath, vertexSrc, fragmentSrc)
	{
		CreateProgram();
	}
	
	OpenGLShader::OpenGLShader(const std::string& filepath) : Shader(filepath)
	{
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ProgramID);
	}

	unsigned int OpenGLShader::CompileShader(GLenum type, const char* shaderSrc)
	{
		unsigned int shaderID = glCreateShader(type);
		glShaderSource(shaderID, 1, &shaderSrc, NULL);
		glCompileShader(shaderID);

		int success;
		char infoLog[512];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		CRESTA_ASSERT(success,"ERROR::SHADER::{0}::COMPILATION_FAILED::{1}",type,infoLog);

		return shaderID;
	}

	void OpenGLShader::CreateProgram()
	{
		m_ProgramID = glCreateProgram();
		if (m_ShaderSrc) 
		{
			for (const auto& pair : *m_ShaderSrc) 
			{
				std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
				const char* shaderSrc = pair.second.c_str();
				
				unsigned int shaderID = CompileShader(pair.first, shaderSrc);
				glAttachShader(m_ProgramID,shaderID);
			}

			glLinkProgram(m_ProgramID);
			int success;
			char infoLog[512];
			glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
			CRESTA_ASSERT(success, "ERROR::LINK::LINK_FAILED::{0}", infoLog)
		}
		else 
		{
			CRESTA_CORE_ERROR("m_ShaderSrc is null!");
		}
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, int count)
	{
		glUniform1iv(glGetUniformLocation(m_ProgramID, name.c_str()), count, values);
	}

	void OpenGLShader::SetFloatArray(const std::string& name, float* values, int count)
	{
		glUniform1fv(glGetUniformLocation(m_ProgramID, name.c_str()), count, values);
	}
	
	void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(glGetUniformLocation(m_ProgramID, name.c_str()), value.x, value.y);
	}
	
	void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(m_ProgramID, name.c_str()), value.x, value.y, value.z);
	}
	
	void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(glGetUniformLocation(m_ProgramID, name.c_str()), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& value)
	{
		glUniformMatrix2fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
}