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
		CRESTA_PROFILE_FUNCTION();
		Entity& entity = CreateEntity(name);
		return entity;
	}

	Entity& Scene::CreateEntity(const std::string& name)
	{
		CRESTA_PROFILE_FUNCTION();
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
		CRESTA_PROFILE_FUNCTION();
		m_Registry.destroy(entity);
		m_EntityMap.erase(entity.GetUUID());
		InvokeSceneUpdateCallBacks();
	}

	void Scene::AssignPhysicsBody(const UUID& entityID)
	{
		CRESTA_PROFILE_FUNCTION();
		Physics::CreateBody(entityID);
	}

	void Scene::AddRigidBody(const UUID& entity)
	{
		CRESTA_PROFILE_FUNCTION();
		Physics::AddRigidBody(entity);
	}

	void Scene::AddCollider(const UUID& entity, const ColliderShape& shape)
	{
		CRESTA_PROFILE_FUNCTION();
		Physics::AddCollider(entity, shape);
	}

	void Scene::RemovePhysicsObject(const UUID& entity, JPH::BodyID& ID)
	{
		CRESTA_PROFILE_FUNCTION();
	}

	void Scene::RemoveRigidBody(const UUID& entity)
	{
		CRESTA_PROFILE_FUNCTION();
		Physics::MakeBodyStatic(entity);
	}

	void Scene::RemoveCollider(const UUID& entity)
	{
		CRESTA_PROFILE_FUNCTION();
		Physics::RemoveCollider(entity);
	}

	void Scene::AddSceneUpdateCallBack(const std::function<void()>& func)
	{
		CRESTA_PROFILE_FUNCTION();
		m_SceneUpdateCallBack.push_back(func);
	}

	void Scene::RemoveSceneUpdateCallBack(const std::function<void()>& func)
	{
		CRESTA_PROFILE_FUNCTION();
	}

	void Scene::InvokeSceneUpdateCallBacks()
	{
		CRESTA_PROFILE_FUNCTION();
		for (auto CallBackFunc : m_SceneUpdateCallBack)
		{
			CallBackFunc();
		}
	}

	void Scene::SerializeScene(const std::filesystem::path& path)
	{
		CRESTA_PROFILE_FUNCTION();
		SceneSerializer::Serialize(*this,path.string());
	}

	Entity* Scene::GetPrimaryCameraEntity()
	{
		CRESTA_PROFILE_FUNCTION();
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
		CRESTA_PROFILE_FUNCTION();
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

	Entity& Scene::FindEntityByID(UUID& ID)
	{
		CRESTA_PROFILE_FUNCTION();
		for (auto& i : m_EntityMap)
		{
			if (i.first == ID)
			{
				return *i.second;
			}
		}
	}

	void Scene::OnRuntimeStart()
	{
		CRESTA_PROFILE_FUNCTION();
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
		CRESTA_PROFILE_FUNCTION();
		std::string tempFolderPath = Application::GetApplication().p_ActiveProjectPath.ProjectFolder.string() + "\\temp";
		// Ensure the folder exists
		if (!std::filesystem::exists(tempFolderPath)) {
			std::filesystem::create_directories(tempFolderPath);
		}

		SerializeScene(tempFolderPath + "\\TempFile.cresta");
	}

	void Scene::Reset()
	{
		CRESTA_PROFILE_FUNCTION();
		std::string tempFolderPath = Application::GetApplication().p_ActiveProjectPath.ProjectFolder.string() + "\\temp";

		SceneSerializer::Deserialize(*this, tempFolderPath + "\\TempFile.cresta", EDIT_ENTITIES);
	}

	void Scene::OnRuntimeStop()
	{
		CRESTA_PROFILE_FUNCTION();
		m_Running = false;
		Physics::Stop();
		for (auto& entity : m_EntityMap)
		{
			entity.second->OnEnd();
		}
		Reset();
	}

	void Scene::FixedUpate()
	{
		CRESTA_PROFILE_FUNCTION();
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
		CRESTA_PROFILE_FUNCTION();
		for (auto& entity : m_EntityMap)
		{
			auto& transform = m_Registry.get<Transform>(*entity.second);
			entity.second->OnRender();
			if (m_Running)
			{
				entity.second->OnUpdate();
			}
			if (m_Registry.has<SpriteRenderer>(*entity.second))
			{
				auto& spriteRenderer = m_Registry.get<SpriteRenderer>(*entity.second);
				Renderer::DrawSprite(spriteRenderer.Color, spriteRenderer.Texture, transform.GetTransform(), spriteRenderer.MixFactor);
			}
		}
	}
}
