#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Cresta 
{	
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
		CRESTA_ASSERT(!success,"ERROR::SHADER::{0}::COMPILATION_FAILED::{1}", type == GL_FRAGMENT_SHADER ? "FRAGMENT SHADER" : "VERTEX SHADER", infoLog);

		return shaderID;
	}

	void OpenGLShader::CreateProgram()
	{
		m_ProgramID = glCreateProgram();
		if (p_ShaderSrc) 
		{
			for (const auto& pair : *p_ShaderSrc) 
			{
				const char* shaderSrc = pair.second.c_str();
				
				unsigned int shaderID = CompileShader(pair.first, shaderSrc);
				glAttachShader(m_ProgramID,shaderID);
			}

			glLinkProgram(m_ProgramID);
			int success;
			char infoLog[512];
			glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
			glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
			CRESTA_ASSERT(!success, "ERROR::LINK::LINK_FAILED::{0}", infoLog)
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

	void OpenGLShader::SetTexture(Ref<Texture2D> texture)
	{
		TextureType type = texture->GetTextureType();
		switch (type)
		{
		case TextureType::NONE:
			break;
		case TextureType::Diffuse:
			texture->Bind((int)type);
			glUniform1i(glGetUniformLocation(m_ProgramID, "u_Diffuse"), (int)type);
			break;
		case TextureType::Specular:
			texture->Bind();
			glUniform1i(glGetUniformLocation(m_ProgramID, "u_Specular"), (int)type);
			break;
		case TextureType::Ambient:
			break;
		case TextureType::Emissive:
			break;
		case TextureType::Height:
			break;
		case TextureType::Normals:
			break;
		case TextureType::Shininess:
			break;
		case TextureType::Opacity:
			break;
		case TextureType::Displacement:
			break;
		case TextureType::Lightmap:
			break;
		case TextureType::Reflection:
			break;
		case TextureType::Base_color:
			break;
		case TextureType::Normal_camera:
			break;
		case TextureType::Emission_color:
			break;
		case TextureType::Metalness:
			break;
		case TextureType::Diffuse_roughness:
			break;
		case TextureType::Ambient_occlusion:
			break;
		case TextureType::Unknown:
			break;
		case TextureType::Sheen:
			break;
		case TextureType::Clearcoat:
			break;
		case TextureType::Transmission:
			break;
		case TextureType::Maya_base:
			break;
		case TextureType::Maya_specular:
			break;
		case TextureType::Maya_specular_color:
			break;
		case TextureType::Maya_specular_roughness:
			break;
		default:
			break;
		}
	}

	void OpenGLShader::SetTexture(Ref<Texture2D>* texture, uint32_t size)
	{
		for (uint32_t i = 0;i < size;i++)
		{
			SetTexture(texture[i]);
		}
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