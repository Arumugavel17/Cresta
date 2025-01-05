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
	private:
		Ref<Scene> m_Scene;
		Entity m_SelectedEntity;
	};

}