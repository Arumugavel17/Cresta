#pragma once

#include "Components.hpp"
#include "Crestaph.hpp"
#include "UUID.hpp"

#include "entt/entt.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/VertexArray.hpp"

namespace Cresta {

	class Entity;

	class Scene
	{
	public:
		Ref<entt::registry> m_Registry;

	public:
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		Scene();
		~Scene();

		static Ref<Scene> Clone(Ref<Scene> other);

		void OnRuntimeStart();
		void OnRuntimeStop();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		Entity FindEntityByName(std::string name);
		Entity GetPrimaryCameraEntity();

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }
		void SetPaused(bool paused) { m_IsPaused = paused; }

		void RenderScene();
		void RenderMeshes();
		void RenderSprits();

		void DestroyEntity(Entity entity);
		void AddSceneUpdateCallBack(const std::function<void()>& func);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		void InvokeSceneUpdateCallBacks();

	private:
		Ref<Shader> m_ModelShader;
		std::unordered_map<UUID, entt::entity> m_EntityMap;
		std::vector<std::function<void()>> m_SceneUpdateCallBack;

		bool m_IsRunning = false;
		bool m_IsPaused = false;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
