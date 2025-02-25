#include "Scene.hpp"
#include "Core/Physics/Physics.hpp"
#include "ECS/Entity.hpp"
#include <glm/glm.hpp>

namespace Cresta 
{
	int Scene::sm_Count = 0;

	Scene::Scene()
	{
		float vertices[] = {
			// Front face
			-1.0f, -1.0f, -1.0f, // 0
			 1.0f, -1.0f, -1.0f, // 1
			 1.0f,  1.0f, -1.0f, // 2
			-1.0f,  1.0f, -1.0f, // 3

			// Back face
			-1.0f, -1.0f,  1.0f, // 4
			 1.0f, -1.0f,  1.0f, // 5
			 1.0f,  1.0f,  1.0f, // 6
			-1.0f,  1.0f,  1.0f  // 7
		};

		uint32_t indices[] = {
			// Front face
			0, 1, 2,  2, 3, 0,
			// Back face
			4, 5, 6,  6, 7, 4,
			// Left face
			0, 3, 7,  7, 4, 0,
			// Right face
			1, 5, 6,  6, 2, 1,
			// Top face
			3, 2, 6,  6, 7, 3,
			// Bottom face
			0, 1, 5,  5, 4, 0
		};

		Cresta::Ref<VertexBuffer> VBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		Cresta::Ref<IndexBuffer> IBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		VBuffer->SetLayout({
			{ShaderDataType::FVec3,"aPos"}
			});
		m_PrimitiveCube = VertexArray::Create();
		m_PrimitiveCube->AddVertexBuffer(VBuffer);
		m_PrimitiveCube->SetIndexBuffer(IBuffer);

		m_Shader = Shader::Create("assets/shaders/FlatShader.glsl");
	}

	Scene::~Scene()
	{
		sm_Count--;
	}

	Entity Scene::CreateEntity(UUID& ID, const std::string& name)
	{
		Entity entity = CreateEntity(name);
		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };

		entity.AddComponent<IDComponent>();
		entity.AddComponent<Transform>();
		
		auto& tag = entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
		m_EntityMap[entity.GetComponent<IDComponent>().GetUUID()] = entity;

		InvokeSceneUpdateCallBacks();
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

	void Scene::Save()
	{
		auto m_RigidbodyView = m_Registry.view<Transform>();
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
		auto m_RigidbodyView = m_Registry.view<Transform>();
		if (m_RigidbodyView.size() > 0)
		{
			for (auto entity : m_RigidbodyView)
			{
				auto& transfromComponent = m_RigidbodyView.get<Transform>(entity);
				transfromComponent.Reset();
			}
		}
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

	Entity* Scene::FindEntityByName(std::string name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
			{
				return new Entity{ entity, this };
			}
		}
		return nullptr;
	}

	Entity* Scene::FindEntityByID(entt::entity entitiyID)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			if (entity == entitiyID)
			{
				return new Entity(entity, this);
			}
		}

		return nullptr;
	}

	void Scene::OnRuntimeStart()
	{
		Save();
		for (auto& entity : m_EntityMap)
		{
			auto& transform = m_Registry.get<Transform>(entity.second);

			if (m_Registry.has<Rigidbody>(entity.second))
			{
				auto& rigidbody = m_Registry.get<Rigidbody>(entity.second);
			}

			Physics::SetBodyPosition(entity.first, transform.Translation);
			Physics::SetBodyRotation(entity.first, transform.Rotation);
		}
		Physics::Start();
		m_Running = true;
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
				if (m_Registry.has<BoxCollider>(entity.second))
				{
					auto& transform = m_Registry.get<Transform>(entity.second);
					if (std::abs(transform.Scale.x)  > 0.1f && std::abs(transform.Scale.y) > 0.1f && std::abs(transform.Scale.z) > 0.1f)
					{
						Physics::SetBodyScale(entity.first, transform.Scale);
					}
				}
			}
			Physics::Step();
		}
	}

	void Scene::RenderScene()
	{
		for (auto& entity : m_EntityMap)
		{
			auto& transform = m_Registry.get<Transform>(entity.second);
			if (m_Registry.has<Rigidbody>(entity.second))
			{
				if (m_Running)
				{
					glm::quat Rotation;
					Physics::GetBodyPosition(entity.first, transform.Translation);
					Physics::GetBodyRotation(entity.first, Rotation);
					transform.SetRotation(Rotation);
				}
			}
			Renderer::DrawGizmoIndexed(m_Shader, m_PrimitiveCube, transform.GetTransform());
			if (m_Registry.has<MeshRenderer>(entity.second))
			{
				auto& meshRenderer = m_Registry.get<MeshRenderer>(entity.second);
				meshRenderer.Draw(transform.GetTransform());
			}
			if (m_Registry.has<SpriteRenderer>(entity.second))
			{
				auto& spriteRenderer = m_Registry.get<SpriteRenderer>(entity.second);
				Renderer::DrawSprite(spriteRenderer.Color, spriteRenderer.Texture, transform.GetTransform(), spriteRenderer.MixFactor);
			}
		}
	}
}
