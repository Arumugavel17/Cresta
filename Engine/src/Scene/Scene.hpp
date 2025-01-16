#pragma once
#include "Crestaph.hpp"
#include "entt/entt.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"

namespace Cresta {

	class Entity;

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

		void OnRuntimeStart();
		void OnRuntimeStop();

		Entity CreateEntity(const std::string& name = std::string());
		Entity FindEntityByName(std::string name);
		Entity GetPrimaryCameraEntity();

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }
		void SetPaused(bool paused) { m_IsPaused = paused; }

		void RenderScene();
		void RenderMeshes();
		void RenderSprits();

		void Step(int frames = 1);
		void DestroyEntity(Entity entity);
		void AddSceneUpdateCallBack(const std::function<void()>& func);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		void InvokeSceneUpdateCallBacks();
		std::vector<std::function<void()>> m_SceneUpdateCallBack;

	private:
		Ref<Shader> m_ModelShader;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
