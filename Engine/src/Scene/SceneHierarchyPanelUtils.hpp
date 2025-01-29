#pragma once

#include "Crestaph.hpp"

namespace Cresta
{
	namespace Utils
	{
		void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	}
}