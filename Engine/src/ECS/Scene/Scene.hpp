#pragma once

#include "Crestaph.hpp"
#include "ECS/UUID.hpp"
#include "ECS/Components/Components.hpp"

#include "entt/entt.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/CubeMap/Cubemap.hpp"

namespace Cresta 
{
	class Entity;
	class Physics;
	class SceneHierarchyPanel;
	class SceneWrapper;

	class Scene
	{
	public:
		entt::registry m_Registry;

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

		Ref<Entity> DuplicateEntity(const Ref<Entity> OriginalEntity,const std::string& name = std::string());
		Ref<Entity> CreateEntity(const std::string& name = std::string(),uint64_t ID = -1);
		Ref<Entity> FindEntityByName(std::string name);
		Ref<Entity> FindEntityByID(UUID& ID);
		Ref<Entity> GetPrimaryCameraEntity();

		bool IsRunning() const { return m_Running; }
		bool IsPaused() const { return m_Paused; }
		void SetPaused(bool paused) { m_Paused = paused; }

		void FixedUpate();
		void OnUpdate();

		void DestroyEntity(Ref<Entity> entity);
		void AddSceneUpdateCallBack(const std::function<void()>& func);
		void RemoveSceneUpdateCallBack(const std::function<void()>& func);

		static void AssignPhysicsBody(const UUID& entity);
		static void AddRigidBody(const UUID& entity);
		static void AddCollider(const UUID& entity, const ColliderShape& shape);

		static void RemovePhysicsObject(const UUID& entity, JPH::BodyID& ID);
		static void RemoveRigidBody(const UUID& entity);
		static void RemoveCollider(const UUID& entity);

		void DrawSkyBox();
		void SerializeScene(const std::filesystem::path& path);

	private:
		void InvokeSceneUpdateCallBacks();

	private:

		std::unordered_map<UUID, Ref<Entity>> m_EntityMap;
		std::vector<std::function<void()>> m_SceneUpdateCallBack;

		bool m_Running = false;
		bool m_Paused = false;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		static int sm_Count;
		CubeMap m_CubeMap;
		
		friend class SceneHierarchyPanel;
		friend class SceneWrapper;
	};
}
