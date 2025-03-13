#include "PrimitiveMeshes.hpp"

namespace Cresta
{
	Ref<Model> Primitive::sm_CubeModel = nullptr;

	void Primitive::Init()
	{
		std::string path = "C:/dev/CrestaProjectFolder/models/box/box.obj";
		sm_CubeModel = Model::Create(path);
	}

	void Primitive::DrawCubeGuizmo(const glm::mat4& position)
	{
		sm_CubeModel->DrawWireFrame(position);
	}
}