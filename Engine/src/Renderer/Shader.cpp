#include "Crestaph.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Cresta 
{
	GLenum Shader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		CRESTA_ASSERT(true, "Unknown shader type!");
		return 0;
	}

	std::string Shader::ReadFile(const std::string& filepath) 
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				CRESTA_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			CRESTA_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	Ref<std::unordered_map<GLenum, std::string>> Shader::PreProcess(const std::string& source)
	{
		auto shaderSources = std::make_shared<std::unordered_map<GLenum, std::string>>();

		const char* typeToken = "//type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			CRESTA_ASSERT(eol == std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			CRESTA_ASSERT(!Shader::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			CRESTA_ASSERT(nextLinePos == std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			(*shaderSources)[Shader::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	Shader::Shader(const std::string& filepath) 
		: p_filepath (filepath)
	{
		const std::string& source = ReadFile(filepath);

		p_ShaderSrc = PreProcess(source);
	}
	
	Shader::Shader(const std::string& filepath, 
				   const std::string& vertexSrc, 
				   const std::string& fragmentSrc) 
		: p_filepath(filepath)
	{
		(*p_ShaderSrc)[GL_VERTEX_SHADER] = vertexSrc;
		(*p_ShaderSrc)[GL_FRAGMENT_SHADER] = fragmentSrc;
	}


	Ref<Shader> Shader::Create(const std::string filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CRESTA_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(filepath);
		}

		CRESTA_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}


	Ref<Shader> Shader::Create(const std::string filepath, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CRESTA_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(filepath, vertexSrc, fragmentSrc);
		}

		CRESTA_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}
	
	// Add the shader to the shader library
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		//with filepath as the name
		auto& filepath = shader->GetFilePath();
		CRESTA_ASSERT(Exists(filepath), "Shader already exists!");
		m_Shaders[filepath] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		CRESTA_ASSERT(Exists(filepath), "Shader already exists!");
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		CRESTA_ASSERT(!Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}