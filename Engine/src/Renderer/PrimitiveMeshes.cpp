#include "PrimitiveMeshes.hpp"
#include "Renderer/Model.hpp"

namespace Cresta
{
	Ref<VertexArray> Primitive::sm_CubeGuizmo = nullptr;
	Ref<VertexArray> Primitive::sm_SphereGuizmo = nullptr;
	Ref<VertexArray> Primitive::sm_CapsuleGuizmo = nullptr;

	Scope<Model> Primitive::sm_SphereModel = nullptr;
	Scope<Model> Primitive::sm_CapsuleModel = nullptr;

	void Primitive::Init()
	{
		sm_SphereModel = CreateScope<Model>("assets/models/sphere/sphere.obj");
		sm_CapsuleModel = CreateScope<Model>("assets/models/sphere/sphere.obj");
	}

	void Primitive::DrawCubeGuizmo()
	{

	}

	void Primitive::DrawSphereGuizmo()
	{

	}

	void Primitive::DrawCapsuleGuizmo()
	{

	}
}