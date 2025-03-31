#include "PhysicsComponents.hpp"
#include "Components.hpp"
#include "ScriptComponent.hpp"
#include "ECS/Scene/SceneHierarchyPanelUtils.hpp"
#include "Core/Application.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>      // Internal functions (required for DockBuilder APIs)
#include <glm/gtc/type_ptr.hpp>

namespace Cresta
{
	inline glm::vec3 Round(const glm::vec3& vec, float num)
	{
		return glm::round(vec * num);
	}

	inline bool HasChanged(const glm::vec3& oldVal, const glm::vec3& newVal, float factor = 1000.0f)
	{
		return Round(oldVal, factor) != Round(newVal, factor);
	}

	void Transform::UI()
	{
		CRESTA_PROFILE_FUNCTION();

		constexpr float roundFactor = 1000.0f;
		if (Cresta::Utils::DrawVec3Control("Translation", m_Translation))
		{
			OnValidate.post(true);
		}

		if (Cresta::Utils::DrawVec3Control("Rotation", rotation))
		{
			glm::quat deltaRotation = glm::quat(glm::radians(rotation)) - m_Rotation;
			m_Rotation += deltaRotation;
			OnValidate.post(true);
		}

		if (Cresta::Utils::DrawVec3Control("Scale", m_Scale, 1.0f))
		{
			OnValidate.post(true);
		}
	}
	
	void SpriteRenderer::UI()
	{
		CRESTA_PROFILE_FUNCTION();

		float availableWidth = ImGui::GetContentRegionAvail().x;  // Get available width for the current window
		float dragWidth = availableWidth * 0.5f;
		ImGui::PushItemWidth(dragWidth);  // Set width for DragFloat
		ImGui::DragFloat("Mix Factor", &MixFactor, 0.1f, 0.0f, 100.0f);
		ImGui::ColorEdit4("Color (Inline)", glm::value_ptr(Color), ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreview);

		ImGui::InputText("Texture", m_Path, 128, ImGuiInputTextFlags_ReadOnly);

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
			if (payload != nullptr)
			{
				// Get the dropped file path
				std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopItemWidth(); // Restore the previous width
	}

	void MeshRenderer::UI()
	{
		CRESTA_PROFILE_FUNCTION();

		char buffer[128];
		std::strncpy(buffer, GetPath().c_str(), sizeof(buffer) - 1);
		buffer[sizeof(buffer) - 1] = '\0';

		if (ImGui::InputText("TextPath", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly))
		{
			SetPath(buffer);
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
			if (payload != nullptr)
			{
				std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);

				SetPath(tempString); 
			}
			ImGui::EndDragDropTarget();
		}
	}
	
	void Rigidbody::UI()
	{
		CRESTA_PROFILE_FUNCTION();
		ImGui::Text("Gravity:");
		ImGui::SameLine();
		if (ImGui::InputFloat("Gravity Factor", &m_GravityFactor))
		{
			SetGravityFactor();
		}
	}

	void BoxCollider::UI()
	{
		CRESTA_PROFILE_FUNCTION();

		if (Cresta::Utils::DrawVec3Control("Size", m_LocalScale, 1.0f))
		{
			Revaluate(p_Entity,true);
		}

		if (Cresta::Utils::DrawVec3Control("Rotation", rotation))
		{
			glm::quat deltaRotation = glm::quat(glm::radians(rotation)) - m_LocalRotation;
			m_LocalRotation += deltaRotation;
			Revaluate(p_Entity, true);
		}

		if (ImGui::Checkbox("IsTrigger", &m_IsTrigger))
		{
			Physics::SetColliderTrigger(p_Entity->GetUUID(), m_IsTrigger);
		}
	}

	void CapsuleCollider::UI()
	{
		CRESTA_PROFILE_FUNCTION();

		ImGui::Text("Capsule Collider");
	}

	void SphereCollider::UI()
	{
		CRESTA_PROFILE_FUNCTION();

		ImGui::Text("Radius:");
		ImGui::SameLine();
		if (ImGui::InputFloat("Radius", &m_Radius))
		{
			Physics::SetBodyShapeScale(p_Entity->GetUUID(),glm::vec3(m_Radius));
		}
	}

	void MeshCollider::UI()
	{
		CRESTA_PROFILE_FUNCTION();

		ImGui::Text("Mesh Collider");
	}

	void AnimatorComponent::UI()
	{
		ImGui::Text("Mesh Collider");
		ImGui::InputFloat("Time Step: ", &m_Animator.timestep);
		
		char buffer[128];
		std::strncpy(buffer, m_Path.c_str(), sizeof(buffer) - 1);
		buffer[sizeof(buffer) - 1] = '\0';

		if (ImGui::InputText("TextPath", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly))
		{
			SetPath(buffer);
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
			if (payload != nullptr)
			{
				std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);
				SetPath(tempString);
			}
			ImGui::EndDragDropTarget();
		}
	}

	void ScriptComponent::UI()
	{
		ImGui::Text("Script Component");

		char buffer[128];
		std::strncpy(buffer, m_ScriptPath.string().c_str(), sizeof(buffer) - 1);
		buffer[sizeof(buffer) - 1] = '\0';

		if (ImGui::InputText("TextPath", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly))
		{
			SetPath(buffer);
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
			if (payload != nullptr)
			{
				std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);
				SetPath(tempString);
			}
			ImGui::EndDragDropTarget();
		}
	}
}