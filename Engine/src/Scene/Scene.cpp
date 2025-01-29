#include "Scene.hpp"
#include "Entity.hpp"

#include <glm/glm.hpp>

namespace Cresta 
{
	int Scene::s_Count = 0;

	Scene::Scene()
	{
		m_Physics = CreateRef<Physics>();
		m_Registry = CreateRef<entt::registry>();
	}

	Scene::~Scene()
	{
		s_Count--;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry->create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<Transform>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		m_EntityMap[uuid] = entity;
		
		InvokeSceneUpdateCallBacks();
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry->destroy(entity);
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

	void Scene::Save()
	{
		auto m_RigidbodyView = m_Registry->view<Transform>();
		if (m_RigidbodyView.size() > 0)
		{
			for (auto entity : m_RigidbodyView)
			{
				auto& transfromComponent = m_RigidbodyView.get<Transform>(entity);
				transfromComponent.Save();
			}
		}
	}

	void Scene::Reset()
	{
		auto m_RigidbodyView = m_Registry->view<Transform>();
		if (m_RigidbodyView.size() > 0)
		{
			for (auto entity : m_RigidbodyView)
			{
				auto& transfromComponent = m_RigidbodyView.get<Transform>(entity);
				transfromComponent.Reset();
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry->view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

	Entity Scene::FindEntityByName(std::string name)
	{
		auto view = m_Registry->view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

	void Scene::OnRuntimeStart()
	{
		Save();
		for (auto entity : m_EntityMap)
		{
			auto transform = m_Registry->get<Transform>(entity.second);
			m_Physics->SetBodyPosition(entity.second, transform.Translation);
		}
		m_Physics->Start();
		m_Running = true;
	}

	void Scene::OnRuntimeStop()
	{
		m_Running = false;
		m_Physics->Stop();
		Reset();
	}

	void Scene::FixedUpate()
	{
		if (m_Running)	
		{
			m_Physics->Step();
		}
	}
	void Scene::RenderScene()
	{
		for (auto entity : m_EntityMap)
		{
			auto transform = m_Registry->get<Transform>(entity.second);
			if (m_Running)
			{
				if (m_Registry->has<PhysicsComponent>(entity.second))
				{
					auto physicsComponent = m_Registry->get<PhysicsComponent>(entity.second); 
					m_Physics->GetBodyPosition(physicsComponent.m_BodyID, transform.Translation);
					glm::quat Rotation;
					m_Physics->GetBodyRotation(physicsComponent.m_BodyID, Rotation);
					transform.SetRotation(Rotation);
				}
			}
			if (m_Registry->has<MeshRenderer>(entity.second))
			{
				auto meshRenderer = m_Registry->get<MeshRenderer>(entity.second);
				if (meshRenderer.model)
				{
					meshRenderer.model->Draw(transform.GetTransform());
				}
			}
			if (m_Registry->has<SpriteRenderer>(entity.second))
			{
				auto spriteRenderer = m_Registry->get<SpriteRenderer>(entity.second);
				Renderer::DrawSprite(spriteRenderer.Color, spriteRenderer.Texture, transform.GetTransform(), spriteRenderer.MixFactor);
			}
		}
	}
	
	//Componenet Added Methods ?////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Transform>(Entity entity, Transform& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRenderer>(Entity entity, SpriteRenderer& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<MeshRenderer>(Entity entity, MeshRenderer& component)	
	{

	}

	PhysicsComponent AddPhysicsComponent(Entity entity)
	{
		PhysicsComponent PhysicsComponent_;
		if (!entity.HasComponent<PhysicsComponent>())
		{
			PhysicsComponent_ = entity.AddComponent<PhysicsComponent>();
		}
		else
		{
			PhysicsComponent_ = entity.GetComponent<PhysicsComponent>();
		}

		return PhysicsComponent_;
	}


	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<PhysicsComponent>(Entity entity, PhysicsComponent& component)
	{
		m_Physics->CreateBody(entity, component.m_BodyID);
	}

	template<>
	void Scene::OnComponentAdded<Rigidbody>(Entity entity, Rigidbody& component)
	{
		PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
		m_Physics->AddRigidBody(PhysicsComponent_.m_BodyID);
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider>(Entity entity, BoxCollider& component)
	{
		PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
		m_Physics->AddCollider(PhysicsComponent_.m_BodyID, ColliderShape::BoxCollider);
	}

	template<>
	void Scene::OnComponentAdded<SphereCollider>(Entity entity, SphereCollider& component)
	{
		PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
		m_Physics->AddCollider(PhysicsComponent_.m_BodyID, ColliderShape::SphereCollider);
	}

	template<>
	void Scene::OnComponentAdded<CapsuleCollider>(Entity entity, CapsuleCollider& component)
	{
		PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
		m_Physics->AddCollider(PhysicsComponent_.m_BodyID, ColliderShape::CapsuleCollider);
	}

	template<>
	void Scene::OnComponentAdded<MeshCollider>(Entity entity, MeshCollider& component)
	{
		PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
	}

	//Componenet Removed Methods ?////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	void Scene::OnComponentRemoved(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentRemoved<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemoved<Transform>(Entity entity, Transform& component)
	{
	}

	template<>
	void Scene::OnComponentRemoved<CameraComponent>(Entity entity, CameraComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemoved<SpriteRenderer>(Entity entity, SpriteRenderer& component)
	{
	}

	template<>
	void Scene::OnComponentRemoved<MeshRenderer>(Entity entity, MeshRenderer& component)
	{

	}

	template<>
	void Scene::OnComponentRemoved<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemoved<PhysicsComponent>(Entity entity, PhysicsComponent& component)
	{

	}

	template<>
	void Scene::OnComponentRemoved<Rigidbody>(Entity entity, Rigidbody& component)
	{
		m_Physics->MakeBodyStatic(entity);
	}

	template<>
	void Scene::OnComponentRemoved<BoxCollider>(Entity entity, BoxCollider& component)
	{
		m_Physics->RemoveCollider(entity);
	}

	template<>
	void Scene::OnComponentRemoved<SphereCollider>(Entity entity, SphereCollider& component)
	{
		m_Physics->RemoveCollider(entity);
	}

	template<>
	void Scene::OnComponentRemoved<CapsuleCollider>(Entity entity, CapsuleCollider& component)
	{
		m_Physics->RemoveCollider(entity);
	}

	template<>
	void Scene::OnComponentRemoved<MeshCollider>(Entity entity, MeshCollider& component)
	{
		m_Physics->RemoveCollider(entity);
	}
}
