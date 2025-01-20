#include "SceneHierarchyPanel.hpp"
#include "Scene/Components.hpp"
#include "SceneHierarchyPanelUtils.hpp"

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
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<MeshRenderer>("Mesh Renderer");
			DisplayAddComponentEntry<Rigidbody>("RigiBody");
			ImGui::EndPopup();
		}

		DrawComponent<TransformComponent>("Transform", m_SelectedEntity, [](auto& component)
			{
				Cresta::Utils::DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				Cresta::Utils::DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				Cresta::Utils::DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", m_SelectedEntity, [](auto& component)
			{
				float availableWidth = ImGui::GetContentRegionAvail().x;  // Get available width for the current window
				float dragWidth = availableWidth * 0.5f;
				ImGui::PushItemWidth(dragWidth);  // Set width for DragFloat
				ImGui::DragFloat("Mix Factor", &component.MixFactor, 0.1f, 0.0f, 100.0f);
				ImGui::ColorEdit4("Color (Inline)", glm::value_ptr(component.Color), ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreview);

				ImGui::InputText("Texture", component.path, 128, ImGuiInputTextFlags_ReadOnly);

				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
					if (payload != nullptr)
					{
						// Get the dropped file path
						std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);
						std::copy(tempString.begin(), tempString.end(), component.path);
						component.path[tempString.size()] = '\0'; // Null-terminate the 
						component.PathChanged();
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PopItemWidth(); // Restore the previous width
			});

		DrawComponent<MeshRenderer>("Mesh Renderer", m_SelectedEntity, [](auto& component)
			{
				
				ImGui::InputText("TextPath", component.path, 128, ImGuiInputTextFlags_ReadOnly);
				
				if(ImGui::BeginDragDropTarget())
				{
					// Accept the drag payload
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
					if (payload != nullptr)
					{
						// Get the dropped file path
						std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);
						std::copy(tempString.begin(), tempString.end(), component.path);
						component.path[tempString.size()] = '\0'; // Null-terminate the 
						component.PathChanged();
					}
					ImGui::EndDragDropTarget();
				}
			});
		DrawComponent<Rigidbody>("RigidBody", m_SelectedEntity, [](auto& component)
			{
				ImGui::Text("RigidBody");
			});
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

	template<typename T, typename UIFunction>
	static void SceneHierarchyPanel::DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

}