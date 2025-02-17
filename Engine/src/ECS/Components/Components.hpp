#pragma once
#include "Crestaph.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Model.hpp"
#include "Renderer/Texture.hpp"
#include "ECS/UUID.hpp"
#include "Core/Physics/PhysicsUtils.hpp"

#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Jolt/Physics/Body/BodyID.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Cresta 
{
	class Entity;

	class Component
	{
	//private:
	//	Entity* m_Entity;
	public:
		template<typename... Dependencies>
		struct Require {};

		virtual ~Component() = default;
		Component() = default;
		//Component(Entity& entity) : m_Entity(&entity){}

		virtual void OnComponentAdded(Entity& entity) {}
		virtual void OnComponentRemoved(Entity& entity) {}
		virtual std::string ToString() = 0;
	};

	class IDComponent
	{
	public:
		UUID m_ID;
	public:
		IDComponent() = default;
		void OnComponentAdded(Entity& entity);
		void OnComponentRemoved() {}

		std::string ToString()
		{
			return "IDComponent";
		}
	};

	class TagComponent : public Component 
	{
	public:
		std::string Tag;

		TagComponent() = default;
		//TagComponent(Entity& entity) : Component(entity) {}
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		void OnComponentAdded(Entity& entity) override;
		std::string ToString() override
		{
			return "Tag Component";
		}
	};

	class Transform : public Component 
	{
	public:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::quat RotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f};

		glm::vec3 Save_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Save_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Save_Scale = { 1.0f, 1.0f, 1.0f };

		Transform() = default;
		//Transform(Entity& entity) : Component(entity) {}
 		Transform(const glm::vec3& translation)
			: Translation(translation) {}

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

		std::string ToString() override
		{
			return "Transform Component";
		}
		
		void OnComponentAdded(Entity& entity) override;
	};

	class SpriteRenderer : public Component 
	{
	public:
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture; 

		char* path = new char[128]();
		float MixFactor = 1.0f;
		SpriteRenderer() = default;
		//SpriteRenderer(Entity& entity) : Component(entity) {}
		SpriteRenderer(const glm::vec4& color)
			: Color(color) {}

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

		std::string ToString() override
		{
			return "Sprite Renderer Component";
		}

		void OnComponentAdded(Entity& entity) override;
	};

	class MeshRenderer : public Component 
	{
	private:
		int m_ID;
		std::string m_Path;
		Ref<Model> m_Model;

	public:
		MeshRenderer() = default;
		//MeshRenderer(Entity& entity) : Component(entity) {}
		MeshRenderer(std::string path,int id) : m_Path(path), m_ID(id)
		{
			PathChanged();
		}

		void PathChanged()
		{
			if (m_Path != "")
			{
				m_Model = Model::Create(m_Path);
			}
		}

		void const SetID(int ID) { m_ID = ID; }
		void const SetPath(std::string path) { m_Path = path; }
		void const SetPath(char* path) { m_Path = std::string(path); }

		inline const int GetID() const { return m_ID; }
		inline const std::string& GetPath() const { return m_Path; }
		inline const Ref<Model>& GetModel() const { return m_Model; }

		void OnComponentAdded(Entity& entity) override;

		std::string ToString() override { return "Mesh Renderer"; }
	};

	class CameraComponent : public Component 
	{
	public:
		Camera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		//CameraComponent(Entity& entity) : Component(entity) {}
		
		std::string ToString() override
		{
			return "Camera Component";
		}

		void OnComponentAdded(Entity& entity) override;
	};
	
	class PhysicsComponent : public Component 
	{
	public:
		JPH::BodyID BodyID; // Physics Body ID 

		PhysicsComponent() = default;
		//PhysicsComponent(Entity& entity) : Component(entity) {}


		std::string ToString() override
		{
			return "Physics Component";
		}

		void OnComponentAdded(Entity& entity) override;
	};

	class Rigidbody : public Component
	{
	public:
		Rigidbody() = default;
		//Rigidbody(Entity& entity) : PhysicsComponent(entity) {}

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

		std::string ToString() override
		{
			return "Rigid Body";
		}

		void OnComponentAdded(Entity& entity) override;
	};


	class Collider : public Component
	{
	public:
		ColliderShape m_Shape;
		Collider() = default;
		Collider(ColliderShape shape) : m_Shape(shape){}
		virtual std::string ToString() override
		{
			return "Collider";
		}
	};

	class BoxCollider : public Collider
	{
	public:
		BoxCollider() : Collider(ColliderShape::BoxCollider) {}

		std::string ToString() override
		{
			return "Box Collider";
		}

		void OnComponentAdded(Entity& entity) override;
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider() : Collider(ColliderShape::SphereCollider) {}

		std::string ToString() override
		{
			return "Sphere Collider";
		}
		void OnComponentAdded(Entity& entity) override;
	};

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider() : Collider(ColliderShape::CapsuleCollider) {}

		std::string ToString() override
		{
			return "Capsule Collider";
		}
		void OnComponentAdded(Entity& entity) override;
	};

	class MeshCollider : public Collider
	{
	public:
		MeshCollider() : Collider(ColliderShape::MeshCollider) {}
		
		std::string ToString() override
		{
			return "Mesh Collider";
		}
		void OnComponentAdded(Entity& entity) override;
	};

	class ClassComponent : public Component
	{
	public:
		ClassComponent() = default;
		//ClassComponent(Entity& entity) : Component(entity) {}	

		std::string ToString() override
		{
			return "Mesh Collider";
		}
		void OnComponentAdded(Entity& entity) override;
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};


	using AllComponents =
		ComponentGroup < Transform, SpriteRenderer, CameraComponent, MeshRenderer, 
		PhysicsComponent, Rigidbody, BoxCollider, SphereCollider, CapsuleCollider, MeshCollider, ClassComponent>;
}
