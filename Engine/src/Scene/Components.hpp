#pragma once
#include "Crestaph.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Model.hpp"
#include "Renderer/Texture.hpp"
#include "UUID.hpp"

#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Jolt/Physics/Body/BodyID.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Cresta 
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) 
		{
		}

		std::string ToString()
		{
			return "Tag Component";
		}
	};

	struct Transform
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::quat RotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f};

		glm::vec3 Save_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Save_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Save_Scale = { 1.0f, 1.0f, 1.0f };

		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::vec3& translation)
			: Translation(translation) {
		}

		void Save()
		{
			Save_Translation = Translation;
			Save_Rotation = Rotation;
			Save_Scale = Scale;
		}

		void Reset()
		{
			Translation = Save_Translation;
			Rotation = Save_Rotation;
			Scale = Save_Scale;
		}

		void SetRotation(const glm::quat& rotation)
		{
			Rotation = glm::eulerAngles(rotation);
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		std::string ToString()
		{
			return "Transform Component";
		}
	};

	struct SpriteRenderer
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture; 

		char* path = new char[128]();
		float MixFactor = 1.0f;

		SpriteRenderer() = default;
		SpriteRenderer(const SpriteRenderer&) = default;
		SpriteRenderer(const glm::vec4& color)
			: Color(color) 
		{
		}

		void PathChanged()
		{
			try
			{
				Ref<Texture2D> ref = Texture2D::Create(path);
				Texture = ref;
			}
			catch (std::exception e)
			{
				CRESTA_CORE_ERROR(e.what());
			}
		}

		std::string ToString()
		{
			return "Sprite Renderer Component";
		}
	};

	struct MeshRenderer
	{
		int ID;
		std::string Path;
		Ref<Model> Model;
		MeshRenderer() = default;
		MeshRenderer(std::string v_path,int id) : Path(v_path), ID(id)
		{
			PathChanged();
		}
		MeshRenderer(const MeshRenderer&) = default;

		void PathChanged()
		{
			if (Path != "")
			{
				Model = Model::Create(Path);
			}
		}

		std::string ToString()
		{
			return "Mesh Renderer";
		}
	};

	struct CameraComponent
	{
		Camera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		std::string ToString()
		{
			return "Camera Component";
		}
	};

	struct PhysicsComponent
	{
		JPH::BodyID BodyID; // Physics Body ID 

		std::string ToString()
		{
			return "Physics Component";
		}
	};

	struct Rigidbody
	{
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Velocity = glm::vec3(0.0f,0.0f,0.0f);
		glm::mat3 RotationMatrix = glm::mat3(1.0f);
		glm::vec3 RotationAxis = glm::vec3(0.0f,1.0f,0.0f);

		void SetTranslation(glm::vec3 translation)
		{
			Translation = translation;
		}

		void SetVelocity(const glm::vec3& velocity)
		{
			Velocity = velocity;
		}

		std::string ToString()
		{
			return "Rigid Body";
		}
	};

	struct BoxCollider
	{
		int temp;
		std::string ToString()
		{
			return "Box Collider";
		}
	};

	struct SphereCollider
	{
		int temp;
		std::string ToString()
		{
			return "Sphere Collider";
		}
	};

	struct CapsuleCollider
	{
		int temp;
		std::string ToString()
		{
			return "Capsule Collider";
		}
	};

	struct MeshCollider
	{
		int temp;

		std::string ToString()
		{
			return "Mesh Collider";
		}
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
		ComponentGroup < Transform, SpriteRenderer, CameraComponent, MeshRenderer, 
		PhysicsComponent, Rigidbody, BoxCollider, SphereCollider, CapsuleCollider, MeshCollider>;
}
