#include "Scene.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "Renderer/Renderer.hpp"

#include <glm/glm.hpp>

namespace Cresta {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		InvokeSceneUpdateCallBacks();
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
		InvokeSceneUpdateCallBacks();
	}

	void Scene::AddSceneUpdateCallBack(const std::function<void()>& func)
	{
		m_SceneUpdateCallBack.push_back(func);
	}

	void Scene::InvokeSceneUpdateCallBacks()
	{
		for (auto CallBackFunc : m_SceneUpdateCallBack)
		{
			CallBackFunc();
		}
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
		RenderMeshes();
		RenderSprits();
	}

	void Scene::RenderMeshes()
	{
		auto group = m_Registry.group<MeshRenderer>(entt::get<TransformComponent>);
		for (auto entity : group)
		{
			auto [transform, Model] = group.get<TransformComponent, MeshRenderer>(entity);
			if (Model.model)
			{
				Model.model->Draw(transform.GetTransform());
			}
		}
	}

	void Scene::RenderSprits()
	{
		auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
		for (auto entity : group)
		{
			auto [transform, Sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer::DrawSprite(Sprite.Color,Sprite.Texture,transform.GetTransform(),Sprite.MixFactor);
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

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Rigidbody>(Entity entity, Rigidbody& component)
	{
	}
}
