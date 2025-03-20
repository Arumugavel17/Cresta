#include "PrimitiveMeshes.hpp"

#include <yaml-cpp/yaml.h>

namespace Cresta
{
	Ref<Model> Primitive::sm_CubeModel = nullptr;

	void Primitive::Init()
	{
		std::string path = "C:/dev/CrestaProjectFolder/models/box/";

		if (std::filesystem::exists(path + "box.data"))
		{
			YAML::Node data;
			data = YAML::LoadFile(path + "box.data");
			uint64_t ModelID = data["ID"].as<uint64_t>();
			sm_CubeModel = Model::Create(path + "box.obj", ModelID);
		}
	}

	void Primitive::DrawCubeGuizmo(const glm::mat4& position)
	{
		sm_CubeModel->DrawWireFrame(position);
	}
}