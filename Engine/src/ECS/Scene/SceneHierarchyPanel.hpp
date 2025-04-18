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

		Ref<Entity>& GetSelectedEntity();
		void SetSelectedEntity(const Ref<Entity>& entity);
		void SetSelectedEntity(entt::entity entity);
	
	private:
		Ref<Camera> m_Camera;
		bool DrawEntityNode(const Ref<Entity>& entity); // Returns False if Entity is nolonger valide i.e it has been deleted
		void DrawInspectorWindow();

	private:
		Ref<Scene> m_Scene;
		Ref<Entity> m_SelectedEntity;
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);
	};
}