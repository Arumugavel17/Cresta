#pragma once
#include "Crestaph.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene/SceneHierarchyPanelUtils.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>      // Internal functions (required for DockBuilder APIs)
#include <glm/gtc/type_ptr.hpp>

namespace Cresta
{
	namespace Utils
	{
		void TransformUI(Transform& component);
		void SpriteRendererUI(SpriteRenderer& component);
		void MeshRendererUI(MeshRenderer& component);
		void RigidbodyUI(Rigidbody& component);
		void RigidbodyUI(Rigidbody& component);
		void BoxColliderUI(BoxCollider& component);
		void CapsuleColliderUI(CapsuleCollider& component);
		void SphereColliderUI(SphereCollider& component);
		void MeshColliderUI(MeshCollider& component);

		template<typename T, typename UIFunction>
		static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
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
}