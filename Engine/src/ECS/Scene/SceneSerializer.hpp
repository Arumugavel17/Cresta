#pragma once

#include "Crestaph.hpp"
#include "Scene.hpp"

#define EDIT_ENTITIES 0
#define CREATE_ENTITIES 1

#include <yaml-cpp/yaml.h>

namespace Cresta 
{
	class SceneSerializer
	{
	public:
		static void Serialize(Scene& scene,const std::string& filepath);
		static bool Deserialize(Scene& scene,const std::string& filepath, int option = CREATE_ENTITIES);
	};

}
