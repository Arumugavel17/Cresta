#include "Crestaph.hpp"
#include "SceneSerializer.hpp"
#include "ECS/Components/Components.hpp"
#include "ECS/UUID.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components/ScriptComponent.hpp"
#include "ECS/Components/AnimationController.hpp"

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

namespace YAML 
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat>
	{
		static Node encode(const glm::quat& rhs)
		{
			Node node;
			node.push_back(rhs.w);
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::quat& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.w = node[0].as<float>();
			rhs.x = node[1].as<float>();
			rhs.y = node[2].as<float>();
			rhs.z = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Cresta::UUID>
	{
		static Node encode(const Cresta::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, Cresta::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};
}



namespace Cresta
{
#define WRITE_SCRIPT_FIELD(FieldType, Type)           \
			case ScriptFieldType::FieldType:          \
				out << scriptField.GetValue<Type>();  \
				break

#define READ_SCRIPT_FIELD(FieldType, Type)             \
	case ScriptFieldType::FieldType:                   \
	{                                                  \
		Type data = scriptField["Data"].as<Type>();    \
		fieldInstance.SetValue(data);                  \
		break;                                         \
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& q)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << q.w << q.x << q.y << q.z << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, entt::entity& entity, entt::registry& registry)
	{
		CRESTA_PROFILE_FUNCTION();

		CRESTA_ASSERT(!registry.has<IDComponent>(entity));

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << (uint64_t) registry.get<IDComponent>(entity).GetUUID();

		if (registry.has<TagComponent>(entity))
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = registry.get<TagComponent>(entity).Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (registry.has<Transform>(entity))
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = registry.get<Transform>(entity);
			out << YAML::Key << "Translation" << YAML::Value << tc.GetPosition();
			out << YAML::Key << "Rotation" << YAML::Value << tc.GetRotation();
			out << YAML::Key << "Scale" << YAML::Value << tc.GetScale();

			out << YAML::EndMap; // TransformComponent
		}

		if (registry.has<SpriteRenderer>(entity))
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = registry.get<SpriteRenderer>(entity);
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			if (spriteRendererComponent.Texture)
				out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.Texture->GetPath();

			out << YAML::Key << "MixFactor " << YAML::Value << spriteRendererComponent.MixFactor ;

			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (registry.has<MeshRenderer>(entity))
		{
			out << YAML::Key << "MeshRenderer";
			out << YAML::BeginMap; // MeshRendererComponent

			auto& MeshRendererComponent = registry.get<MeshRenderer>(entity);
			out << YAML::Key << "path" << YAML::Value << MeshRendererComponent.GetPath();
			out << YAML::EndMap; 
		}

		if (registry.has<AnimatorComponent>(entity))
		{
			out << YAML::Key << "Animator";
			out << YAML::BeginMap; // AnimatorComponent

			auto& animatorComponent = registry.get<AnimatorComponent>(entity);
			out << YAML::Key << "path" << YAML::Value << animatorComponent.GetPath();
			out << YAML::EndMap; 
		}

		if (registry.has<Rigidbody>(entity))
		{
			out << YAML::Key << "Rigidbody";
			out << YAML::BeginMap; 
			out << YAML::EndMap; 
		}
		if (registry.has<BoxCollider>(entity))
		{
			auto& BoxCollComp = registry.get<BoxCollider>(entity);

			out << YAML::Key << "BoxCollider";
			out << YAML::BeginMap;
			out << YAML::Key << "Rotation" << YAML::Value << BoxCollComp.GetRotation();
			out << YAML::Key << "Size" << YAML::Value << BoxCollComp.GetSize();
			out << YAML::Key << "Trigger" << YAML::Value << BoxCollComp.IsTrigger();
			out << YAML::EndMap; 
		}
		if (registry.has<SphereCollider>(entity))
		{
			out << YAML::Key << "SphereCollider";
			out << YAML::BeginMap; 
			out << YAML::EndMap; //
		}
		if (registry.has<CapsuleCollider>(entity))
		{
			out << YAML::Key << "CapsuleCollider";
			out << YAML::BeginMap; 
			out << YAML::EndMap; //
		}

		if (registry.has<ScriptComponent>(entity))
		{
			auto& ScriptComp = registry.get<ScriptComponent>(entity);

			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Path" << YAML::Value << ScriptComp.GetPath();
			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(Scene& scene, const std::string& filepath)
	{
		CRESTA_PROFILE_FUNCTION();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		scene.m_Registry.each([&](auto entityID)
			{
				SerializeEntity(out, entityID, scene.m_Registry);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(Scene& scene, const std::string& filepath, int option)
	{
		CRESTA_PROFILE_FUNCTION();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			CRESTA_CORE_ERROR("Failed to load .cresta file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		CRESTA_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				CRESTA_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				if (option == CREATE_ENTITIES)
				{
					Ref<Entity> deserializedEntity = scene.CreateEntity(name,uuid);

					auto transformComponent = entity["TransformComponent"];
					if (transformComponent)
					{
						// Entities always have transforms
						auto& tc = deserializedEntity->GetComponent<Transform>();
						tc.SetPosition(transformComponent["Translation"].as<glm::vec3>());
						tc.SetRotation(transformComponent["Rotation"].as<glm::quat>());
						tc.SetScale(transformComponent["Scale"].as<glm::vec3>());
					}

					auto spriteRendererComponent = entity["SpriteRendererComponent"];
					if (spriteRendererComponent)
					{
						auto& src = deserializedEntity->AddComponent<SpriteRenderer>();
						src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
						if (spriteRendererComponent["TexturePath"])
						{
							std::string texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
							src.Texture = Texture2D::Create(texturePath);
						}

						if (spriteRendererComponent["MixFactor "])
						{
							src.MixFactor = spriteRendererComponent["MixFactor "].as<float>();
						}
					}

					auto MeshRendererComponenet = entity["MeshRenderer"];
					if (MeshRendererComponenet)
					{
						int64_t ModelID = -1;
						std::string path = MeshRendererComponenet["path"].as<std::string>();
						if (!path.empty())
						{
							std::string filepath = path.substr(0,path.find_last_of("."));
							filepath = filepath + ".data";
							if (std::filesystem::exists(filepath))
							{
								YAML::Node data;
								data = YAML::LoadFile(filepath);
								ModelID = data["ID"].as<uint64_t>();
							}

							auto& comp = deserializedEntity->AddComponent<MeshRenderer>(path, ModelID);
						}
					}

					auto animatorComponent = entity["Animator"];
					if (animatorComponent)
					{
						auto& animator = deserializedEntity->AddComponent<AnimatorComponent>();
						animator.SetPath(animatorComponent["path"].as<std::string>());
					}

					auto RigibodyComponent = entity["Rigidbody"];
					if (RigibodyComponent)
					{
						auto& rigidbody = deserializedEntity->AddComponent<Rigidbody>();
					}

					auto SphereColliderComponent = entity["SphereCollider"];
					if (SphereColliderComponent)
					{
						auto& spherecollider = deserializedEntity->AddComponent<SphereCollider>();
					}

					auto CapsuleColliderComponenet = entity["CapsuleCollider"];
					if (CapsuleColliderComponenet)
					{
						auto& Capsulecollider = deserializedEntity->AddComponent<CapsuleCollider>();
					}

					auto BoxColliderComponent = entity["BoxCollider"];
					if (BoxColliderComponent)
					{
						auto& boxcollider = deserializedEntity->AddComponent<BoxCollider>();
						auto Center = BoxColliderComponent["Center"];
						auto Rotation = BoxColliderComponent["Rotation"];
						auto Size = BoxColliderComponent["Size"];
						auto Trigger = BoxColliderComponent["Trigger"];

						boxcollider.SetRotation({
							Rotation[0].as<float>(),
							Rotation[1].as<float>(),
							Rotation[2].as<float>(),
							Rotation[3].as<float>()
							});

						boxcollider.SetScale({
							Size[0].as<float>(),
							Size[1].as<float>(),
							Size[2].as<float>()
							});

						boxcollider.SetTrigger(Trigger.as<bool>());
					}

					auto ScriptComp = entity["ScriptComponent"];
					if (ScriptComp)
					{
						auto& script = deserializedEntity->AddComponent<ScriptComponent>();
						script.SetPath(ScriptComp["Path"].as<std::string>());

					}
				}
				else if(option == EDIT_ENTITIES)
				{
					Ref<Entity> deserializedEntity = scene.FindEntityByID(*(new UUID(uuid)));

					auto transformComponent = entity["TransformComponent"];
					if (transformComponent)
					{
						// Entities always have transforms
						auto& tc = deserializedEntity->GetComponent<Transform>();
						tc.SetPosition(transformComponent["Translation"].as<glm::vec3>());
						tc.SetRotation(transformComponent["Rotation"].as<glm::quat>());
						tc.SetScale(transformComponent["Scale"].as<glm::vec3>());
					}

					auto spriteRendererComponent = entity["SpriteRendererComponent"];
					if (spriteRendererComponent)
					{
						auto& src = deserializedEntity->GetComponent<SpriteRenderer>();
						src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
						if (spriteRendererComponent["TexturePath"])
						{
							std::string texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
							src.Texture = Texture2D::Create(texturePath);
						}

						if (spriteRendererComponent["MixFactor "])
						{
							src.MixFactor = spriteRendererComponent["MixFactor "].as<float>();
						}
					}

					auto MeshRendererComponenet = entity["MeshRenderer"];
					if (MeshRendererComponenet)
					{
						std::string path = MeshRendererComponenet["path"].as<std::string>();

						auto& comp = deserializedEntity->GetComponent<MeshRenderer>();
					}

					auto RigibodyComponent = entity["Rigidbody"];
					if (RigibodyComponent)
					{
						auto& rigidbody = deserializedEntity->GetComponent<Rigidbody>();
					}

					auto SphereColliderComponent = entity["SphereCollider"];
					if (SphereColliderComponent)
					{
						auto& spherecollider = deserializedEntity->GetComponent<SphereCollider>();
					}

					auto CapsuleColliderComponenet = entity["CapsuleCollider"];
					if (CapsuleColliderComponenet)
					{
						auto& Capsulecollider = deserializedEntity->GetComponent<CapsuleCollider>();
					}

					auto BoxColliderComponent = entity["BoxCollider"];
					if (BoxColliderComponent)
					{
						auto& boxcollider = deserializedEntity->GetComponent<BoxCollider>();
						auto Center = BoxColliderComponent["Center"];
						auto Rotation = BoxColliderComponent["Rotation"];
						auto Size = BoxColliderComponent["Size"];
						auto Trigger = BoxColliderComponent["Trigger"];


						boxcollider.SetRotation({
							Rotation[0].as<float>(),
							Rotation[1].as<float>(),
							Rotation[2].as<float>(),
							Rotation[3].as<float>()
							});

						boxcollider.SetScale({
							Size[0].as<float>(),
							Size[1].as<float>(),
							Size[2].as<float>()
							});

						boxcollider.SetTrigger(Trigger.as<bool>());
					}

					auto ScriptComp = entity["ScriptComponent"];
					if (ScriptComp)
					{
						auto& script = deserializedEntity->GetComponent<ScriptComponent>();
						//script.SetPath(ScriptComp["Path"].as<std::string>());
					}
				}
			}
		}

		return true;
	}
}