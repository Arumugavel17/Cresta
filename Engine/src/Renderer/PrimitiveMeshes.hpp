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

		static void DrawSphereGuizmo(const glm::vec3& position, const glm::quat& rotation, float radius);
		static void PrimitiveSphere(Ref<Shader> shader, float radius);

		static void DrawCapsuleGuizmo(const glm::mat4& transform);
		static void PrimitiveCapsule(Ref<Shader> shader, const glm::mat4& transform);

	private:
		static Ref<VertexArray> sm_BoxVAO;
		static Ref<VertexArray> sm_CapsuleVAO;
		static Ref<VertexArray> sm_SphereVAO;

		static Ref<Shader> sm_PrimitiveShader;

		static float sm_SphereSectors;
		static float sm_SphereStacks;
		static float sm_SphereRadius;

		static float sm_CapsuleSectors;
		static float sm_CapsuleHeight;
		static float sm_CapsuleRadius;

		static Ref<Model> sm_CapsuleModel;
	};
}