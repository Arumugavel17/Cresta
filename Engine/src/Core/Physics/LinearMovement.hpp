#pragma once
#include "Crestaph.hpp"
#include "Scene/Components.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Cresta
{
	void SetUpCuboid(Rigidbody& body )
	{
		body.Translation.x = 0;
		body.Translation.y = 1.5;
		body.Translation.z = 0;

		body.Velocity.x = 1.0;
		body.Velocity.y = 0;
		body.Velocity.z = 1.0;
	}

	glm::mat3 Cross(const glm::vec3& vector)
	{
		return glm::mat3(
			0.0,		-vector.z,	vector.y,
			vector.z,	0.0,		-vector.x,
		   -vector.y,	vector.x,	0.0
		);
	}

	void StepSimulation(Transform& transfrom,Rigidbody& body, float DeltaTime)
	{
		transfrom.Translation += (DeltaTime  * body.Velocity);
		body.RotationMatrix += (DeltaTime * Cross(body.RotationAxis) * body.RotationMatrix);
		glm::extractEulerAngleXYZ(glm::mat4(body.RotationMatrix),transfrom.Rotation.x, transfrom.Rotation.y, transfrom.Rotation.z);

		transfrom.Translation = (DeltaTime * body.RotationMatrix) * transfrom.Translation;
		body.Translation = transfrom.Translation;
	}
}