#pragma once
#include "Crestaph.hpp"
#include "ECS/Scene/Scene.hpp"
#include "ECS/Entity.hpp"
#include "Renderer/Camera.hpp"

namespace Cresta
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);
		void SetEditorCamera(Ref<Camera> EditorCamera) { m_Camera = EditorCamera; }
		void PrepareEntity();

		void SetScene(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity* GetSelectedEntity();
		void SetSelectedEntity(Entity& entity);
		void SetSelectedEntity(entt::entity entity);
	
	private:
		Ref<Camera> m_Camera;
		void DrawEntityNode(Entity& entity);
		void DrawInspectorWindow();

	private:
		Ref<Scene> m_Scene;
		Entity* m_SelectedEntity;
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);
	};
}