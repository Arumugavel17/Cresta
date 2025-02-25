#include "PrimitiveMeshes.hpp"

namespace Cresta
{
	Scope<Model> Primitive::sm_CubeModel = nullptr;
	Scope<Model> Primitive::sm_SphereModel = nullptr;
	Scope<Model> Primitive::sm_CapsuleModel = nullptr;

	void Primitive::Init()
	{
		sm_CubeModel = CreateScope<Model>("assets/models/sphere/sphere.obj");
		sm_SphereModel = CreateScope<Model>("assets/models/sphere/sphere.obj");
		sm_CapsuleModel = CreateScope<Model>("assets/models/sphere/sphere.obj");
	}

	void Primitive::DrawCubeGuizmo(const glm::vec3& position)
	{
		sm_CubeModel->DrawWireFrame(position);
	}

	void Primitive::DrawSphereGuizmo(const glm::vec3& position)
	{
		sm_SphereModel->DrawWireFrame(position);
	}

	void Primitive::DrawCapsuleGuizmo(const glm::vec3& position)
	{
		sm_CapsuleModel->DrawWireFrame(position);
	}
}