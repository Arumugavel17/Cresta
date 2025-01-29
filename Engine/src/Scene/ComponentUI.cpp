#include "ComponentUI.hpp"

namespace Cresta
{
	namespace Utils
	{
		void TransformUI(Transform& component)
		{
			Cresta::Utils::DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			Cresta::Utils::DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			Cresta::Utils::DrawVec3Control("Scale", component.Scale, 1.0f);
		}

		void SpriteRendererUI(SpriteRenderer& component)
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
		}

		void MeshRendererUI(MeshRenderer& component)
		{
			char buffer[128];
			std::strncpy(buffer, component.path.c_str(), sizeof(buffer) - 1);
			buffer[sizeof(buffer) - 1] = '\0';

			if (ImGui::InputText("TextPath", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly))
			{
				component.path = buffer;
				component.PathChanged();
			}

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
				if (payload != nullptr)
				{
					std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);
					component.path = tempString; // Assign directly to the std::string
					component.PathChanged();
				}
				ImGui::EndDragDropTarget();
			}
		}

		void RigidbodyUI(Rigidbody& component)
		{
			ImGui::Text("RigidBody");
		}
		void BoxColliderUI(BoxCollider& component)
		{
			ImGui::Text("Box Collider");
		}
		void CapsuleColliderUI(CapsuleCollider& component)
		{
			ImGui::Text("Capsule Collider");
		}
		void SphereColliderUI(SphereCollider& component)
		{
			ImGui::Text("Sphere Collider");
		}

		void MeshColliderUI(MeshCollider& component)
		{
			ImGui::Text("Mesh Collider");
		}
	}
}