#pragma once
#include "Crestaph.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Model.hpp"
#include "Renderer/Texture.hpp"
#include "UUID.hpp"

#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Cresta {

	struct Component {

	};

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
			: Tag(tag) {
		}

		std::string ToString()
		{
			return "Tag Component";
		}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {
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

	struct SpriteRendererComponent
	{
		char* path = new char[128]();
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture; 
		float MixFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
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
		char* path = new char[128]();
		Ref<Model> model;
		MeshRenderer() = default;
		MeshRenderer(std::string v_path)
		{
			path = v_path.data();
			PathChanged(); 
		}
		MeshRenderer(const MeshRenderer&) = default;

		void PathChanged()
		{
			if (Model::s_ModelsLoaded.find(std::string(path)) != Model::s_ModelsLoaded.end())
			{
				model = Model::s_ModelsLoaded[std::string(path)];
			}
			else
			{
				model = Model::Create(path);
			}
		}

		std::string ToString()
		{
			return "Mesh Renderer";
		}
	};

	struct Rigidbody
	{
		float Mass;
		bool UseGravity;
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

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
		ComponentGroup < TransformComponent, SpriteRendererComponent, CameraComponent, MeshRenderer>;
}
