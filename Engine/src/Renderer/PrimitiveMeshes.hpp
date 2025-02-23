#pragma once
#include "Crestaph.hpp"
#include "Renderer/Renderer.hpp"

namespace Cresta
{
	class Primitive
	{
	public:
		static void Init();
		static void DrawCubeGuizmo();
		static void DrawSphereGuizmo();
		static void DrawCapsuleGuizmo();
	private:
		static Ref<VertexArray> sm_CubeGuizmo;
		static Ref<VertexArray> sm_SphereGuizmo;
		static Ref<VertexArray> sm_CapsuleGuizmo;

		static Scope<Model> sm_SphereModel;
		static Scope<Model> sm_CapsuleModel;
	};
}