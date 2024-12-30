#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Cresta {
	
	OpenGLShader::OpenGLShader(const std::string& filepath) : Shader(filepath)
	{
		m_ProgramID = glCreateProgram();

		unsigned int shader_id;
		shader_id = glCreateShader(GL_VERTEX_SHADER);

		std::ifstream shader_file(filepath);
		std::stringstream buffer;

		if (!shader_file.is_open())
		{
			std::cerr << "File Unable to open \n returned -1" << std::endl;
			return;
		}

		buffer << shader_file.rdbuf();
		std::cout << buffer.str() << "\n";
		std::string str = buffer.str();
		const char* c_str = str.c_str();

		/*
		you can join above two lines and write
		const char* c_str = buffer.str().c_str();
		However, there's a caveat: when using buffer.str().c_str(), str() returns a temporary
		std::string object, and calling c_str() on it will give a pointer to this temporary object,
		which might be destroyed after the statement, leading to undefined behavior. Instead, you should
		store the result of buffer.str() in a separate variable to ensure the pointer remains valid.
		*/


		glShaderSource(shader_id, 1, &c_str, NULL);

		glCompileShader(shader_id);

		int success;
		char infoLog[512];
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << GL_VERTEX_SHADER << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		glAttachShader(m_ProgramID, shader_id);

		shader_file.close();

	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ProgramID);
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