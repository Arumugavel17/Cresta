#include "SceneHierarchyPanel.hpp"
#include "Scene/Components.hpp"
#include "Scene/ComponentUI.hpp"

#include "Core/Input.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>      // Internal functions (required for DockBuilder APIs)
#include <imgui/imgui_impl_glfw.h>   // Platform-specific (GLFW example)
#include <imgui/imgui_impl_opengl3.h> // Renderer-specific (OpenGL example)

#include <glm/gtc/type_ptr.hpp>

namespace Cresta
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetScene(scene);
	}

	void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_SelectedEntity = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_Scene)
		{
			m_Scene->m_Registry->each([&](auto entityID)
				{
					Entity entity{ entityID , m_Scene.get() };
					DrawEntityNode(entity);
				});

			// Deselect entity when clicking on blank space
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
			{
				m_SelectedEntity = {};
			}

			if (Input::GetKeyDown(Key::LeftControl) && Input::GetKeyDown(Key::N))
			{
				m_Scene->CreateEntity("Empty Entity"); 
			}

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow("SceneHierarchyBlankContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
				{
					m_Scene->CreateEntity("Empty Entity");
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();

		DrawInspectorWindow();
	}

	Entity SceneHierarchyPanel::GetSelectedEntity() const
	{
		return m_SelectedEntity;
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectedEntity = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Scene->DestroyEntity(entity);
			if (m_SelectedEntity == entity)
			{
				m_SelectedEntity = {};
			}
		}
	}

	void SceneHierarchyPanel::DrawInspectorWindow()
	{
		ImGui::Begin("Inspector");
		if (!m_SelectedEntity || !m_SelectedEntity.HasComponent<TagComponent>())
		{
			ImGui::End();
			return;
		}

		auto& tag = m_SelectedEntity.GetComponent<TagComponent>().Tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));

		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRenderer>("Sprite Renderer");
			DisplayAddComponentEntry<MeshRenderer>("Mesh Renderer");
			DisplayAddComponentEntry<Rigidbody>("RigiBody");
			DisplayAddComponentEntry<BoxCollider>("BoxCollider");
			DisplayAddComponentEntry<SphereCollider>("SphereCollider");
			DisplayAddComponentEntry<CapsuleCollider>("CapsuleCollider");
			DisplayAddComponentEntry<MeshCollider>("MeshCollider");
			ImGui::EndPopup();
		}

		Utils::DrawComponent<Transform>("Transform", m_SelectedEntity, Utils::TransformUI);
		Utils::DrawComponent<SpriteRenderer>("Sprite Renderer", m_SelectedEntity, Utils::SpriteRendererUI);
		Utils::DrawComponent<MeshRenderer>("Mesh Renderer", m_SelectedEntity, Utils::MeshRendererUI);
		Utils::DrawComponent<Rigidbody>("RigidBody", m_SelectedEntity, Utils::RigidbodyUI);
		Utils::DrawComponent<BoxCollider>("BoxCollider", m_SelectedEntity, Utils::BoxColliderUI);
		Utils::DrawComponent<SphereCollider>("SphereCollider", m_SelectedEntity, Utils::SphereColliderUI);
		Utils::DrawComponent<CapsuleCollider>("CapsuleCollider", m_SelectedEntity, Utils::CapsuleColliderUI);
		Utils::DrawComponent<MeshCollider>("MeshCollider", m_SelectedEntity, Utils::MeshColliderUI);

		ImGui::End();
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectedEntity.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectedEntity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}