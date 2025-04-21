#pragma once
#include "Crestaph.hpp"
#include "RendererComponents.hpp"
#include "PhysicsComponents.hpp"
#include "ECS/Entity.hpp"
#include "Core/Input.hpp"

namespace Cresta
{
	struct vec3
	{
	public:
		float x = 0.0f, y = 0.0f, z = 0.0f;

		operator glm::vec3() const {
			return glm::vec3(x,y,z);
		}

		// Addition
		vec3 operator+(const vec3& v) const {
			return vec3{ x + v.x, y + v.y, z + v.z };
		}

		// Subtraction
		vec3 operator-(const vec3& v) const {
			return vec3{x - v.x, y - v.y, z - v.z};
		}

		// Multiplication by scalar
		vec3 operator*(float scalar) const {
			return vec3{x * scalar, y * scalar, z * scalar};
		}

		// Scalar multiplication (scalar * vec3)
		friend vec3 operator*(float scalar, const vec3& v) {
			return v * scalar;
		}

		// Division by scalar
		vec3 operator/(float scalar) const {
			return vec3{x / scalar, y / scalar, z / scalar};
		}

		// Equality
		bool operator==(const vec3& v) const {
			return x == v.x && y == v.y && z == v.z;
		}

		// Inequality
		bool operator!=(const vec3& v) const {
			return !(*this == v);
		}

		// Compound assignment operators
		vec3& operator+=(const vec3& v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		vec3& operator-=(const vec3& v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}

		vec3& operator*=(float scalar) {
			x *= scalar; y *= scalar; z *= scalar;
			return *this;
		}

		vec3& operator/=(float scalar) {
			x /= scalar; y /= scalar; z /= scalar;
			return *this;
		}

		// Dot product
		float dot(const vec3& v) const {
			return x * v.x + y * v.y + z * v.z;
		}

		// Cross product
		vec3 cross(const vec3& v) const {
			return vec3{
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
			};
		}

		// Output stream operator for debugging
		friend std::ostream& operator<<(std::ostream& os, const vec3& v) {
			return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		}
	};

	struct quat
	{
		quat() = default;
		quat(float v_w, float v_x, float v_y, float v_z) : w(v_w), x(v_x), y(v_y), z(v_z)
		{ }
		float w = 1.0f,x = 0.0f, y = 0.0f, z = 0.0f;

		operator glm::quat() const {
			return glm::quat(w, x, y, z);
		}

		quat operator*(const quat& other) const {
			glm::quat q1 = *this;
			glm::quat q2 = other;
			glm::quat result = q1 * q2;
			return { result.w,result.x, result.y, result.z };
		}

		quat& operator*=(const quat& other) {
			*this = *this * other;
			return *this;
		}

		quat operator+(const quat& other) const {
			return { w + other.w, x + other.x, y + other.y, z + other.z };
		}

		quat& operator+=(const quat& other) {
			w += other.w;
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		quat operator*(float scalar) const {
			return { w * scalar, x * scalar, y * scalar, z * scalar};
		}

		quat& operator*=(float scalar) {
			w *= scalar;
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		bool operator==(const quat& other) const {
			return w == other.w && x == other.x && y == other.y && z == other.z;
		}

		bool operator!=(const quat& other) const {
			return !(*this == other);
		}

		static quat fromAxisAngle(vec3 axis, float angle) {
			float halfAngle = angle * 0.5f;
			float sinHalfAngle = sin(halfAngle);

			return quat(
				cos(halfAngle),
				axis.x * sinHalfAngle,
				axis.y * sinHalfAngle,
				axis.z * sinHalfAngle
			);
		}
	};


	inline vec3 CreateVec3(float x, float y, float z)
	{
		return vec3{ x, y, z };
	}

	inline quat CreateQuat(float w, float x, float y, float z)
	{
		return quat{w, x, y, z };
	}

	class TransformWrapper
	{
	public:

		inline void SetPosition(const vec3& Position) { TransformComponent->PhysicsSetPosition({ Position.x,Position.y,Position.z }); }
		inline void SetScale(const vec3& Scale) { TransformComponent->PhysicsSetScale({ Scale.x,Scale.y,Scale.z }); }
		inline void SetRotation(const quat& Rotation) { TransformComponent->PhysicsSetRotation({ Rotation.w, Rotation.x,Rotation.y,Rotation.z }); }

		inline constexpr vec3& GetPosition() const
		{
			glm::vec3 position = TransformComponent->GetPosition();
			return *(new vec3{ position.x, position.y, position.z });
		}

		inline constexpr quat& GetRotation() const
		{
			glm::quat rotation = TransformComponent->GetRotation();
			return *(new quat{ rotation.w, rotation.x, rotation.y, rotation.z });
		}

		inline constexpr vec3& GetScale() const
		{
			glm::vec3 scale = TransformComponent->GetScale();
			return *(new vec3{ scale.x, scale.y, scale.z });
		}

		Transform* TransformComponent;
	};

	
	class SpriteRendererWrapper
	{
	public:
		void SetPath(std::string path)
		{
			//m_Entity->GetComponent<SpriteRenderer>().SetPath(path);
		}
	};

	class SceneWrapper;
	class BoxColliderWrapper;
	class MeshRendererWrapper;
	class EntityWrapper
	{
	public:
		EntityWrapper(Entity* entity);
		TransformWrapper& GetTransform();
		void AttachMeshRenderer();
		void AttachBoxCollider();
		void AttachScript(std::string path);
		void PrintHandle()
		{
			std::cout << (uint32_t)*p_Entity << "\n";
		}

	private:
		Entity* p_Entity = nullptr;
		TransformWrapper m_TransformWrapper;

		friend class SceneWrapper;
		friend class BoxColliderWrapper;
		friend class MeshRendererWrapper;
	};

	class SceneWrapper
	{
	public:
		SceneWrapper();
		EntityWrapper CreateEntity();
		bool DestroyEntity(EntityWrapper entity);
	private:
		Ref<Scene> m_Scene;
	};

	class MeshRendererWrapper
	{
	public:
		static void SetPath(EntityWrapper entity,std::string path)
		{
			
			if (entity.p_Entity->HasComponent<MeshRenderer>())
			{
				entity.p_Entity->GetComponent<MeshRenderer>().SetPath(path);
			}
			else 
			{
				CRESTA_ASSERT("MeshRenderer Is Not Attached");
			}
		}
	};

	class BoxColliderWrapper
	{
	public:
		void SetScale(EntityWrapper entity, vec3 scale)
		{
			entity.p_Entity->GetComponent<BoxCollider>().SetScale(scale);
		}

		void SetRotation(EntityWrapper entity, quat rotation)
		{
			entity.p_Entity->GetComponent<BoxCollider>().SetRotation(rotation);
		}
	};
}