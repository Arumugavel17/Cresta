#include "Crestaph.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Cresta {

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CRESTA_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(filepath);
		}

		CRESTA_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CRESTA_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		//case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		CRESTA_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
	// Add the shader to the shader library
	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		//with filepath as the name
		auto& filepath = shader->GetFilePath();
		CRESTA_ASSERT(!Exists(filepath), "Shader already exists!");
		m_Shaders[filepath] = shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		CRESTA_ASSERT(!Exists(filepath), "Shader already exists!");
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		CRESTA_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}