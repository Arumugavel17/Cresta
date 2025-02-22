#pragma once

#include "Scene/Scene.hpp"
#include "Components/ComponentHeader.hpp"
#include "entt/entt.hpp"

namespace Cresta 
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene)
		{
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{

			CRESTA_ASSERT(HasComponent<T>(), "Entity already has component!");

			// Create the component and add it to the registry
			T& component = m_Scene->m_Registry->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

			// If T is derived from Component, call OnComponentAdded
			component.OnComponentAdded(*this);

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
			component.OnComponentRemoved(*this);
			m_Scene->m_Registry->remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		UUID GetUUID() { return GetComponent<IDComponent>().m_ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		void OnUpdate();
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
