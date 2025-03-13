#pragma once
#include "Crestaph.hpp"
#include "Renderer/Renderer.hpp"

namespace Cresta
{
	class Primitive
	{
	public:
		static void Init();

		static void DrawCubeGuizmo(const glm::mat4& position);
		
	private:
		static Ref<Model> sm_CubeModel;
	};
}