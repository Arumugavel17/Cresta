#include "Components.hpp"
#include "ECS/Scene/SceneHierarchyPanelUtils.hpp"
#include "cmath"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>      // Internal functions (required for DockBuilder APIs)
#include <glm/gtc/type_ptr.hpp>

namespace Cresta
{
	static glm::vec3 TransformPosition = glm::vec3(0.0f);
	static glm::vec3 TransformRotation = glm::vec3(0.0f);
	static glm::vec3 TransformScale = glm::vec3(0.0f);

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
		constexpr float roundFactor = 1000.0f;
		if (Cresta::Utils::DrawVec3Control("Translation", m_Translation))
		{
			OnValidate.post();
		}

		glm::vec3 rotation = glm::degrees(m_Rotation);
		
		if (Cresta::Utils::DrawVec3Control("Rotation", rotation))
		{
			OnValidate.post();
		}

		m_Rotation = glm::radians(rotation);
		
		if (Cresta::Utils::DrawVec3Control("Scale", m_Scale, 1.0f))
		{
			OnValidate.post();
		}

		TransformPosition = m_Translation;
		TransformRotation = m_Rotation;
		TransformScale = m_Scale;
	}
	
	void SpriteRenderer::UI()
	{
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
		char buffer[128];
		std::strncpy(buffer, GetPath().c_str(), sizeof(buffer) - 1);
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
	
	void Rigidbody::UI()
	{
		ImGui::Text("RigidBody");
	}

	void BoxCollider::UI()
	{
		if (Cresta::Utils::DrawVec3Control("Size", m_OffestLocalScale, 1.0f))
		{
			Revaluate(p_Entity);
		}
		if (Cresta::Utils::DrawVec3Control("Rotation", m_OffestLocalRotation, 1.0f))
		{
			Revaluate(p_Entity);
		}
		if (ImGui::Checkbox("IsTrigger", &m_IsTrigger))
		{
			Physics::SetColliderTrigger(p_Entity->GetUUID(), m_IsTrigger);
		}
	}

	void CapsuleCollider::UI()
	{
		ImGui::Text("Capsule Collider");
	}

	void SphereCollider::UI()
	{
		ImGui::Text("Sphere Collider");
	}

	void MeshCollider::UI()
	{
		ImGui::Text("Mesh Collider");
	}

}