#include "SceneHierarchyPanel.hpp"
#include "Scene/Components.hpp"

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
			m_Scene->m_Registry.each([&](auto entityID)
				{
					Entity entity{ entityID , m_Scene.get() };
					DrawEntityNode(entity);
				});

			// Deselect entity when clicking on blank space
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
				m_SelectedEntity = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow("SceneHierarchyBlankContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Scene->CreateEntity("Empty Entity");

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
				entityDeleted = true;

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
				m_SelectedEntity = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		// Define reusable styles
		ImVec4 resetButtonColor = { 0.8f, 0.1f, 0.15f, 1.0f };
		ImVec4 resetButtonHoverColor = { 0.9f, 0.2f, 0.2f, 1.0f };
		ImVec4 resetButtonActiveColor = { 0.8f, 0.1f, 0.15f, 1.0f };

		ImVec4 xButtonColor = { 0.2f, 0.7f, 0.2f, 1.0f };
		ImVec4 xButtonHoverColor = { 0.3f, 0.8f, 0.3f, 1.0f };
		ImVec4 xButtonActiveColor = { 0.2f, 0.7f, 0.2f, 1.0f };

		ImVec4 zButtonColor = { 0.1f, 0.25f, 0.8f, 1.0f };
		ImVec4 zButtonHoverColor = { 0.2f, 0.35f, 0.9f, 1.0f };
		ImVec4 zButtonActiveColor = { 0.1f, 0.25f, 0.8f, 1.0f };

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		// Render label
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 8.0f, 4.0f });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 4.0f, lineHeight };

		float availableWidth = ImGui::GetContentRegionAvail().x;  // Get available width for the current window
		// Function to create a styled button with drag float
		auto createStyledButton = [&](std::string buttonLabel, ImVec4 color, ImVec4 hoverColor, ImVec4 activeColor, float* value) {
			ImGui::PushStyleColor(ImGuiCol_Button, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
			ImGui::PushFont(boldFont);

			if (ImGui::Button(buttonLabel.substr(2,-1).c_str(), buttonSize))
				*value = resetValue;

			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			std::string dragFloatID = buttonLabel + "DrawFloat";

			float dragWidth = availableWidth * 0.2f;  
			ImGui::PushItemWidth(dragWidth);  // Set width for DragFloat
			ImGui::DragFloat(dragFloatID.c_str(), value, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth(); // Restore the previous width
			};

		// Render X button and value
		createStyledButton("##X", resetButtonColor, resetButtonHoverColor, resetButtonActiveColor, &values.x);
		ImGui::SameLine();
		// Render Y button and value
		createStyledButton("##Y", xButtonColor, xButtonHoverColor, xButtonActiveColor, &values.y);
		ImGui::SameLine();
		// Render Z button and value
		createStyledButton("##Z", zButtonColor, zButtonHoverColor, zButtonActiveColor, &values.z);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
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
			ImGui::EndPopup();
		}

		DrawComponent<TransformComponent>("Transform", m_SelectedEntity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", m_SelectedEntity, [](auto& component)
			{
				float availableWidth = ImGui::GetContentRegionAvail().x;  // Get available width for the current window
				float dragWidth = availableWidth * 0.5f;
				ImGui::PushItemWidth(dragWidth);  // Set width for DragFloat
				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
				ImGui::ColorEdit4("Color (Inline)", glm::value_ptr(component.Color), ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreview);

				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath(path);
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
							component.Texture = texture;
						else
							CRESTA_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PopItemWidth(); // Restore the previous width
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