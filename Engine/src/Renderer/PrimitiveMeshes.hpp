#pragma once
#include "Crestaph.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/VertexArray.hpp"

namespace Cresta
{
	class Primitive
	{
	public:
		static void Init();

		static void DrawCubeGuizmo(const glm::mat4& transform);
		static void PrimitiveCube(Ref<Shader> shader, const glm::mat4& transform);
		
	private:
		static Ref<VertexArray> sm_VAO;
		static Ref<Model> sm_CubeModel;
		static Ref<Shader> sm_PrimitiveShader;
	};
}