#pragma once
#include "Crestaph.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"

namespace Cresta
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetScene(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const;
		void SetSelectedEntity(Entity entity);
	
	private:
		void DrawEntityNode(Entity entity);
		void DrawInspectorWindow();
	private:
		Ref<Scene> m_Scene;
		Entity m_SelectedEntity;

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);
		template<typename T, typename UIFunction>
		static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction);
	};

}