#include "RendererComponents.hpp"
#include "Renderer/PrimitiveMeshes.hpp"
#include "ECS/Entity.hpp"
#include "Core/Time.hpp"

#include <yaml-cpp/yaml.h>

namespace Cresta
{
	void SpriteRenderer::OnComponentAdded()
	{
		CRESTA_INFO("SpriteRenderer OnComponentAdded");
	}

	std::map<MeshRenderer::ShaderType,Ref<Shader>> MeshRenderer::sm_Shader;
	MeshRenderer::MeshRenderer(Entity* entity, const std::string& path,int64_t Model_ID) : ComponentTemplate(entity), m_Path(path)
	{
		if (!sm_Shader[ShaderType::NORMAL])
		{
			sm_Shader[ShaderType::NORMAL] = Shader::Create("assets/shaders/Model.glsl");
		}

		m_ID = static_cast<int>(entt::entity(*entity));
		if (Model_ID == -1)
		{
			m_ModelID = new UUID();
			CreateModelDataFile();
		}
		else
		{
			m_ModelID = new UUID(Model_ID);
		}
		
		PathChanged();
	}

	void MeshRenderer::CreateModelDataFile()
	{
		std::string model_name = m_Path.substr(m_Path.find_last_of("\\") + 1, m_Path.length());
		model_name = model_name.substr(0,model_name.find_last_of("."));
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Model" << YAML::Value << model_name;
		out << YAML::Key << "Path" << YAML::Value << m_Path;
		out << YAML::Key << "ID" << YAML::Value << (uint64_t)*m_ModelID;
		out << YAML::EndMap;

		std::string directoriy = m_Path.substr(0, m_Path.find_last_of("\\"));
		std::string filepath = directoriy + '\\' + model_name + ".data";
		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void MeshRenderer::OnRender()
	{
		CRESTA_PROFILE_FUNCTION();

		Draw(p_Entity->GetComponent<Transform>().GetTransform());
	}

	void MeshRenderer::OnComponentAdded()
	{
		CRESTA_INFO("Mesh Renderer OnComponentAdded");
	}

	void SpriteRenderer::OnComponentRemoved()
	{
		CRESTA_INFO("SpriteRenderer OnComponentRemoved");
	}

	void MeshRenderer::OnComponentRemoved()
	{
		CRESTA_INFO("Mesh Renderer OnComponentRemoved");
	}

}