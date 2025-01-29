#pragma once

#include "Scene.hpp"
#include "Components.hpp"
#include "entt/entt.hpp"

namespace Cresta {

	class Entity
	{
	public:
		Entity() = default;
		Entity::Entity(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene)
		{
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			CRESTA_ASSERT(HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			CRESTA_ASSERT(!HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry->get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry->has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			CRESTA_ASSERT(!HasComponent<T>(), "Entity does not have component!");
			T& component = m_Scene->m_Registry->get<T>(m_EntityHandle);
			m_Scene->OnComponentRemoved<T>(*this, component);
			m_Scene->m_Registry->remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }


		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};

}
