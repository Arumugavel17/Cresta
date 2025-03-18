#include "Components.hpp"
#include "ECS/Scene/SceneHierarchyPanelUtils.hpp"
#include "cmath"
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

		glm::vec3 rotation = glm::degrees(glm::eulerAngles(m_Rotation)); // Convert to degrees

		if (Cresta::Utils::DrawVec3Control("Rotation", rotation))
		{
			m_Rotation = glm::quat(glm::radians(rotation)); // Convert back to radians before creating quaternion
			OnValidate.post(true);
		}


		m_Rotation = glm::radians(rotation);
		
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

		ImGui::Text("RigidBody");
	}

	void BoxCollider::UI()
	{
		CRESTA_PROFILE_FUNCTION();

		//if (!m_SceneIsActive)
		//{
		//	if (Cresta::Utils::DrawVec3Control("Center", m_Center, 1.0f))
		//	{
		//		Revaluate(p_Entity);
		//	}
		//}
		if (Cresta::Utils::DrawVec3Control("Size", m_LocalScale, 1.0f))
		{
			Revaluate(p_Entity,true);
		}
		glm::vec3 rotation = glm::degrees(glm::eulerAngles(m_LocalRotation));
		if (Cresta::Utils::DrawVec3Control("Rotation", rotation))
		{
			m_LocalRotation = glm::quat(glm::radians(rotation));
			Revaluate(p_Entity,true);
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

		ImGui::Text("Sphere Collider");
	}

	void MeshCollider::UI()
	{
		CRESTA_PROFILE_FUNCTION();

		ImGui::Text("Mesh Collider");
	}

	void AnimatorComponent::UI()
	{
		ImGui::Text("Mesh Collider");

		char buffer[128];
		std::strncpy(buffer, m_Path.c_str(), sizeof(buffer) - 1);
		buffer[sizeof(buffer) - 1] = '\0';

		if (ImGui::InputText("TextPath", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly))
		{
			SetPath(buffer);
			PathChanged();
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
			if (payload != nullptr)
			{
				std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);
				SetPath(tempString);
				PathChanged();
			}
			ImGui::EndDragDropTarget();
		}
	}
}