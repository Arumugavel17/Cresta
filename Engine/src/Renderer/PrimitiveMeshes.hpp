#pragma once
#include "Crestaph.hpp"
#include "Renderer/Renderer.hpp"

namespace Cresta
{
	class Primitive
	{
	public:
		static void Init();
		static void DrawCubeGuizmo(const glm::vec3& position);
		static void DrawSphereGuizmo(const glm::vec3& position);
		static void DrawCapsuleGuizmo(const glm::vec3& position);

	private:
		static Scope<Model> sm_CubeModel;
		static Scope<Model> sm_SphereModel;
		static Scope<Model> sm_CapsuleModel;
	};
}