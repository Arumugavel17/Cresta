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
	}
}