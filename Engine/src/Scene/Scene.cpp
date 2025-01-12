#include "Crestaph.hpp"
#include "Scene.hpp"
#include "Entity.hpp"

#include "Components.hpp"
#include "Renderer/Renderer.hpp"
#include <glm/glm.hpp>

namespace Cresta {

	Scene::Scene()
	{
		m_VertexArray = VertexArray::Create();
		m_Shader = Shader::Create("assets/shaders/FlatShader.glsl");
		m_ModelShader = Shader::Create("assets/shaders/FlatShader.glsl");
		float Vertices[] = {
			-0.5f, -0.5f, 0.0f, 
			 0.5f, -0.5f, 0.0f, 
			 0.5f,  0.5f, 0.0f, 
			-0.5f,  0.5f, 0.0f  
		};

		unsigned int Indices[] = {
			0, 1, 2, // First triangle
			2, 3, 0  // Second triangle
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(Vertices, sizeof(Vertices));
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(Indices, 6);

		vertexBuffer->SetLayout({
				{ ShaderDataType::FVec3 , "aPos" }
			});

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	Scene::~Scene()
	{
		//delete m_PhysicsWorld;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::Step(int frames)
	{
		m_StepFrames = frames;
	}

	Entity Scene::FindEntityByName(std::string name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::RenderScene()
	{
		// Draw sprites
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<MeshRenderer>);
			for (auto entity : group)
			{
				auto [transform, Model] = group.get<TransformComponent, MeshRenderer>(entity);
				if (Model.model) 
				{
					Model.model->Draw(transform.GetTransform());
				}
			}
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<MeshRenderer>(Entity entity, MeshRenderer& component)	
	{
		//component.model = CreateRef<Model>("assets/models/backpack/backpack.obj");	
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}
}
