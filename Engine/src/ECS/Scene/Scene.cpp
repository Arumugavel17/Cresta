#include "Scene.hpp"
#include "Core/Application.hpp"
#include "Core/Physics/Physics.hpp"
#include "Renderer/PrimitiveMeshes.hpp"
#include "ECS/Scene/SceneSerializer.hpp"
#include "ECS/Entity.hpp"
#include <glm/glm.hpp>

namespace Cresta 
{
	int Scene::sm_Count = 0;

	Scene::Scene()
	{
		Physics::ClearBodies();
	}

	Scene::~Scene()
	{
		sm_Count--;
	}

	Entity& Scene::CreateEntity(UUID& ID, const std::string& name)
	{
		Entity& entity = CreateEntity(name);
		return entity;
	}

	Entity& Scene::CreateEntity(const std::string& name)
	{
		Ref<Entity> entity1 = CreateRef<Entity>(m_Registry.create(), this);
		Entity& entity = *entity1;
		entity.AddComponent<IDComponent>();
		entity.AddComponent<Transform>();
		
		auto& tag = entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
		m_EntityMap[entity.GetComponent<IDComponent>().GetUUID()] = entity1;

		InvokeSceneUpdateCallBacks();

		int num = m_Registry.size();
		CRESTA_TRACE("{}", num);

		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
		InvokeSceneUpdateCallBacks();
	}

	void Scene::AssignPhysicsBody(const UUID& entityID)
	{
		Physics::CreateBody(entityID);
	}

	void Scene::AddRigidBody(const UUID& entity)
	{
		Physics::AddRigidBody(entity);
	}

	void Scene::AddCollider(const UUID& entity, const ColliderShape& shape)
	{
		Physics::AddCollider(entity, shape);
	}

	void Scene::RemovePhysicsObject(const UUID& entity, JPH::BodyID& ID)
	{
		
	}

	void Scene::RemoveRigidBody(const UUID& entity)
	{
		Physics::MakeBodyStatic(entity);
	}

	void Scene::RemoveCollider(const UUID& entity)
	{
		Physics::RemoveCollider(entity);
	}

	void Scene::AddSceneUpdateCallBack(const std::function<void()>& func)
	{
		m_SceneUpdateCallBack.push_back(func);
	}

	void Scene::RemoveSceneUpdateCallBack(const std::function<void()>& func)
	{

	}

	void Scene::InvokeSceneUpdateCallBacks()
	{
		for (auto CallBackFunc : m_SceneUpdateCallBack)
		{
			CallBackFunc();
		}
	}

	void Scene::SerializeScene(const std::filesystem::path& path)
	{
		SceneSerializer::Serialize(*this,path.string());
	}

	Entity* Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				return new Entity{ entity, this };
			}
		}
		return nullptr;
	}

	Entity& Scene::FindEntityByName(std::string name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
			{
				return *m_EntityMap[m_Registry.get<IDComponent>(entity).GetUUID()];
			}
		}
	}

	Entity& Scene::FindEntityByID(entt::entity entitiyID)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			if (entity == entitiyID)
			{
				return *m_EntityMap[m_Registry.get<IDComponent>(entity).GetUUID()];
			}
		}
	}

	void Scene::OnRuntimeStart()
	{
		Save();
		for (auto& entity : m_EntityMap)
		{
			entity.second->OnStart();
		}
		Physics::Start();
		m_Running = true;
	}

	void Scene::Save()
	{
		std::string tempFolderPath = Application::GetApplication().p_ActiveProjectPath.second.string() + "\\temp";
		// Ensure the folder exists
		if (!std::filesystem::exists(tempFolderPath)) {
			std::filesystem::create_directories(tempFolderPath);
		}

		SerializeScene(tempFolderPath + "\\TempFile.cresta");
	}

	void Scene::Reset()
	{
		std::string tempFolderPath = Application::GetApplication().p_ActiveProjectPath.second.string() + "\\temp";

		SceneSerializer::Deserialize(*this, tempFolderPath + "\\TempFile.cresta", EDIT_ENTITIES);
	}

	void Scene::OnRuntimeStop()
	{
		m_Running = false;
		Physics::Stop();
		Reset();
	}

	void Scene::FixedUpate()
	{
		if (m_Running)	
		{
			for (auto& entity : m_EntityMap)
			{
				entity.second->OnFixedUpdate();
			}
			Physics::Step();
		}
	}

	void Scene::OnUpdate()
	{
		for (auto& entity : m_EntityMap)
		{
			auto& transform = m_Registry.get<Transform>(*entity.second);	
			entity.second->OnUpdate();

			if (m_Registry.has<SpriteRenderer>(*entity.second))
			{
				auto& spriteRenderer = m_Registry.get<SpriteRenderer>(*entity.second);
				Renderer::DrawSprite(spriteRenderer.Color, spriteRenderer.Texture, transform.GetTransform(), spriteRenderer.MixFactor);
			}
		}
	}
}
