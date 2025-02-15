#include "ECS/Scene/Scene.hpp"
#include "ECS/Entity.hpp"

#include "Core/Physics/LinearMovement.hpp"

#include <glm/glm.hpp>

#define DEFINE_ON_COMPONENT_ADDED(Type, Code)      \
template<>                                         \
void Cresta::Scene::OnComponentAdded<Type>(Entity entity, Type& component) \
{                                                  \
    Code                                           \
}

#define NO_OP {}  // For components that need no special handling


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
		m_Physics = CreateScope<Physics>();
		m_Registry = CreateRef<entt::registry>();
	}

	Scene::~Scene()
	{
		sm_Count--;
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

	Entity* Scene::FindEntityByID(entt::entity entitiyID)
	{
		auto view = m_Registry->view<TagComponent>();
		for (auto entity : view)
		{
			if (entity == entitiyID)
			{
				return new Entity(entity, this);
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

			if (m_Registry->has<Rigidbody>(entity.second))
			{
				auto& rigidbody = m_Registry->get<Rigidbody>(entity.second);
				SetUpCuboid(rigidbody);
			}

			if (m_Registry->has<BoxCollider>(entity.second))
			{
				auto& boxCollider = m_Registry->get<BoxCollider>(entity.second);
				m_Physics->SetBodyScale(entity.first, transform.Scale);
			}

			m_Physics->SetBodyPosition(entity.first, transform.Translation);
			m_Physics->SetBodyRotation(entity.first, transform.Rotation);
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
			for (auto entity : m_EntityMap)
			{
				if (m_Registry->has<BoxCollider>(entity.second))
				{
					auto transform = m_Registry->get<Transform>(entity.second);
					if (std::abs(transform.Scale.x)  > 0.1f && std::abs(transform.Scale.y) > 0.1f && std::abs(transform.Scale.z) > 0.1f)
					{
						m_Physics->SetBodyScale(entity.first, transform.Scale);
					}
				}
			}
			m_Physics->Step();
		}
	}

	void Scene::RenderScene()
	{
		for (auto entity : m_EntityMap)
		{
			auto& transform = m_Registry->get<Transform>(entity.second);
			if (m_Registry->has<PhysicsComponent>(entity.second))
			{
				if (m_Running)
				{
					glm::quat Rotation;
					auto physicsComponent = m_Registry->get<PhysicsComponent>(entity.second); 
					m_Physics->GetBodyPosition(physicsComponent.BodyID, transform.Translation);
					m_Physics->GetBodyRotation(physicsComponent.BodyID, Rotation);
					transform.SetRotation(Rotation);
				}
				Renderer::DrawGizmoIndexed(m_Shader, m_PrimitiveCube, transform.GetTransform());
			}
			if (m_Registry->has<MeshRenderer>(entity.second))
			{
				auto meshRenderer = m_Registry->get<MeshRenderer>(entity.second);
				if (meshRenderer.Model)
				{
					meshRenderer.Model->Draw(transform.GetTransform(),meshRenderer.ID);
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

	// Then in your .cpp:
	DEFINE_ON_COMPONENT_ADDED(IDComponent, NO_OP)
	DEFINE_ON_COMPONENT_ADDED(Transform, NO_OP)
	DEFINE_ON_COMPONENT_ADDED(CameraComponent, NO_OP)
	DEFINE_ON_COMPONENT_ADDED(SpriteRenderer, NO_OP)
	DEFINE_ON_COMPONENT_ADDED(MeshRenderer, { component.ID = static_cast<int>(entity); })

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
	DEFINE_ON_COMPONENT_ADDED(TagComponent, NO_OP)
	DEFINE_ON_COMPONENT_ADDED(PhysicsComponent, { m_Physics->CreateBody(entity.GetComponent<IDComponent>().ID, component.BodyID); } )
	DEFINE_ON_COMPONENT_ADDED(Rigidbody, {
			PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
			m_Physics->AddRigidBody(PhysicsComponent_.BodyID);
		})
	DEFINE_ON_COMPONENT_ADDED(BoxCollider, {
			PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
			m_Physics->AddCollider(PhysicsComponent_.BodyID, ColliderShape::BoxCollider);
		})
	DEFINE_ON_COMPONENT_ADDED(SphereCollider, {
			PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
			m_Physics->AddCollider(PhysicsComponent_.BodyID, ColliderShape::SphereCollider); 
		})
	DEFINE_ON_COMPONENT_ADDED(CapsuleCollider, {
			PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
			m_Physics->AddCollider(PhysicsComponent_.BodyID, ColliderShape::CapsuleCollider);
		})
	DEFINE_ON_COMPONENT_ADDED(MeshCollider, {
			PhysicsComponent PhysicsComponent_ = AddPhysicsComponent(entity);
		})

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
		m_Physics->MakeBodyStatic(entity.GetComponent<IDComponent>().ID);
	}

	template<>
	void Scene::OnComponentRemoved<BoxCollider>(Entity entity, BoxCollider& component)
	{
		m_Physics->RemoveCollider(entity.GetComponent<IDComponent>().ID);
	}

	template<>
	void Scene::OnComponentRemoved<SphereCollider>(Entity entity, SphereCollider& component)
	{
		m_Physics->RemoveCollider(entity.GetComponent<IDComponent>().ID);
	}

	template<>
	void Scene::OnComponentRemoved<CapsuleCollider>(Entity entity, CapsuleCollider& component)
	{
		m_Physics->RemoveCollider(entity.GetComponent<IDComponent>().ID);
	}

	template<>
	void Scene::OnComponentRemoved<MeshCollider>(Entity entity, MeshCollider& component)
	{
		m_Physics->RemoveCollider(entity.GetComponent<IDComponent>().ID);
	}
}
