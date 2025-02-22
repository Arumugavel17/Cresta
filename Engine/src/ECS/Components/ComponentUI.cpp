#include "ComponentUI.hpp"
#include "cmath"

namespace Cresta
{
	namespace Utils
	{
		static glm::vec3 TransformPosition = glm::vec3(0.0f);
		static glm::vec3 TransformRotation = glm::vec3(0.0f);
		static glm::vec3 TransformScale = glm::vec3(0.0f);

		inline glm::vec3 Round(const glm::vec3& vec,float num)
		{
			return glm::round(vec * num);
		}

		inline bool HasChanged(const glm::vec3& oldVal, const glm::vec3& newVal, float factor = 1000.0f) 
		{
			return Round(oldVal, factor) != Round(newVal, factor);
		}

		void TransformUI(Transform& component)
		{
			constexpr float roundFactor = 1000.0f;
			Cresta::Utils::DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			Cresta::Utils::DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			Cresta::Utils::DrawVec3Control("Scale", component.Scale, 1.0f);


			if (HasChanged(TransformPosition, component.Translation, roundFactor))
			{
				CRESTA_INFO("Position Changed {0} {1} {2} : {3} {4} {5}",
					TransformPosition.x, TransformPosition.y, TransformPosition.z,
					component.Translation.x, component.Translation.y, component.Translation.z);
			}

			if (HasChanged(TransformRotation, component.Rotation, roundFactor))
			{
				CRESTA_INFO("Rotation Changed {0} {1} {2} : {3} {4} {5}",
					TransformRotation.x, TransformRotation.y, TransformRotation.z,
					component.Rotation.x, component.Rotation.y, component.Rotation.z);
			}

			if (HasChanged(TransformScale, component.Scale, roundFactor))
			{
				CRESTA_INFO("Scale Changed {0} {1} {2} : {3} {4} {5}",
					TransformScale.x, TransformScale.y, TransformScale.z,
					component.Scale.x, component.Scale.y, component.Scale.z);
			}

			TransformPosition = component.Translation;
			TransformRotation = component.Rotation;
			TransformScale = component.Scale;
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
			std::strncpy(buffer, component.GetPath().c_str(), sizeof(buffer) - 1);
			buffer[sizeof(buffer) - 1] = '\0';

			if (ImGui::InputText("TextPath", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly))
			{
				component.SetPath(buffer);
				component.PathChanged();
			}

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
				if (payload != nullptr)
				{
					std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);
					component.SetPath(tempString); // Assign directly to the std::string
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