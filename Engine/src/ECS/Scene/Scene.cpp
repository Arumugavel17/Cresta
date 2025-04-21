#include "Scene.hpp"
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
		std::vector<std::string> CubeMapPaths;
		CubeMapPaths.push_back("assets\\Textures\\Daylight\\Daylight Box_Right.bmp");
		CubeMapPaths.push_back("assets\\Textures\\Daylight\\Daylight Box_Left.bmp");
		CubeMapPaths.push_back("assets\\Textures\\Daylight\\Daylight Box_Top.bmp");
		CubeMapPaths.push_back("assets\\Textures\\Daylight\\Daylight Box_Bottom.bmp");
		CubeMapPaths.push_back("assets\\Textures\\Daylight\\Daylight Box_Front.bmp");
		CubeMapPaths.push_back("assets\\Textures\\Daylight\\Daylight Box_Back.bmp");

		Physics::ClearBodies();
		m_CubeMap.SetTextures(CubeMapPaths);
	}

	Scene::~Scene()
	{
		sm_Count--;
	}
	
	Ref<Entity> Scene::DuplicateEntity(const Ref<Entity> OriginalEntity, const std::string& name)
	{
		Ref<Entity> newEntity = CreateEntity(OriginalEntity->GetTage());
		return newEntity;
	}


	Ref<Entity> Scene::CreateEntity(const std::string& name, uint64_t ID)
	{
		CRESTA_PROFILE_FUNCTION();
		Ref<Entity> entity = CreateRef<Entity>(m_Registry.create(), this);
		if (ID == -1) 
		{
			entity->AddComponent<IDComponent>();
		}
		else
		{
			entity->AddComponent<IDComponent>(ID);
		}
		entity->AddComponent<Transform>();
		
		auto& tag = entity->AddComponent<TagComponent>(name.empty() ? "Entity" : name);
		m_EntityMap[entity->GetComponent<IDComponent>().GetUUID()] = entity;

		InvokeSceneUpdateCallBacks();

		int num = m_Registry.size();
		CRESTA_TRACE("{}", num);

		return entity;
	}

	void Scene::DestroyEntity(Entity* entity)
	{
		CRESTA_PROFILE_FUNCTION();
		const UUID& ID = entity->GetUUID();
		m_EntityMap[ID]->CleanUp();
		m_Registry.destroy((entt::entity)*entity);
		m_EntityMap[ID].~shared_ptr();
		m_EntityMap.erase(ID);
		InvokeSceneUpdateCallBacks();
	}

	void Scene::DestroyEntity(Ref<Entity> entity)
	{
		CRESTA_PROFILE_FUNCTION();
		const UUID& ID = entity->GetUUID();
		m_EntityMap[ID]->CleanUp();
		m_EntityMap[ID].~shared_ptr();
		m_Registry.destroy((entt::entity)*entity);
		m_EntityMap.erase(ID);
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

	Ref<Entity> Scene::GetPrimaryCameraEntity()
	{
		CRESTA_PROFILE_FUNCTION();
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				return CreateRef<Entity>(entity, this);
			}
		}
		return nullptr;
	}

	Ref<Entity> Scene::FindEntityByName(std::string name)
	{
		CRESTA_PROFILE_FUNCTION();
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
			{
				return m_EntityMap[m_Registry.get<IDComponent>(entity).GetUUID()];
			}
		}
	}

	Ref<Entity> Scene::FindEntityByID(UUID& ID)
	{
		CRESTA_PROFILE_FUNCTION();
		for (auto& i : m_EntityMap)
		{
			if (i.first == ID)
			{
				return i.second;
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

	void Scene::DrawSkyBox()
	{
		m_CubeMap.Draw();
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
