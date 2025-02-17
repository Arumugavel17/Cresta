#pragma once

#include "Crestaph.hpp"
#include "ECS/UUID.hpp"
#include "ECS/Components/Components.hpp"

#include "entt/entt.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/VertexArray.hpp"
#include "Core/Physics/Physics.hpp"

namespace Cresta 
{
	class Entity;

	class Scene
	{
	public:
		Ref<entt::registry> m_Registry;
		static Scope<Physics> m_Physics;

	public:
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		Scene();
		~Scene();

		void Save();
		void Reset();
		void OnRuntimeStart();
		void OnRuntimeStop();

		Entity& CreateEntity(const std::string& name = std::string());
		Entity& CreateEntity(UUID& ID,const std::string& name = std::string());
		Entity* FindEntityByName(std::string name);
		Entity* FindEntityByID(entt::entity entitiyID);

		Entity* GetPrimaryCameraEntity();

		bool IsRunning() const { return m_Running; }
		bool IsPaused() const { return m_Paused; }
		void SetPaused(bool paused) { m_Paused = paused; }

		void FixedUpate();
		void RenderScene();

		void DestroyEntity(Entity& entity);
		void AddSceneUpdateCallBack(const std::function<void()>& func);
		void RemoveSceneUpdateCallBack(const std::function<void()>& func);

		void AddPhysicsObject(const UUID& entity, JPH::BodyID& ID);
		void AddRigidBody(const JPH::BodyID& ID);
		void Scene::AddCollider(const JPH::BodyID& ID, const ColliderShape& shape);

	private:
		void InvokeSceneUpdateCallBacks();

	private:
		std::unordered_map<UUID, Entity> m_EntityMap;
		std::vector<std::function<void()>> m_SceneUpdateCallBack;

		bool m_Running = false;
		bool m_Paused = false;

		Ref<VertexArray> m_PrimitiveCube;
		Ref<Shader> m_Shader;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		static int sm_Count;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
