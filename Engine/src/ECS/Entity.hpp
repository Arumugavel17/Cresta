#pragma once

#include "Scene/Scene.hpp"
#include "Components/ComponentHeader.hpp"
#include "entt/entt.hpp"
#include "Core/Events/Event.hpp"

namespace Cresta 
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {}
		Entity(const Entity& other)
		{
			OnFixedUpdateFunctions = other.OnFixedUpdateFunctions;
			OnUpdateFunctions = other.OnUpdateFunctions;
			m_EntityHandle = other.m_EntityHandle;
			m_Scene = other.m_Scene;
		}

		~Entity()
		{
			std::cout << "\n";
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			CRESTA_ASSERT(HasComponent<T>(), "Entity already has component!");

			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle,this, std::forward<Args>(args)...);
			component.OnComponentAdded();

			if constexpr (has_overridden_OnStart<ComponentTemplate, T>::value)
			{
				OnStartFunctions.emplace_back(typeid(T).name(), [&]() { this->GetComponent<T>().OnStart(); });
			}

			if constexpr (has_overridden_OnRender<ComponentTemplate, T>::value)
			{
				OnRenderFunctions.emplace_back(typeid(T).name(), [&]() { this->GetComponent<T>().OnRender(); });
			}

			if constexpr (has_overridden_OnUpdate<ComponentTemplate, T>::value)
			{
				OnUpdateFunctions.emplace_back(typeid(T).name(), [&]() { this->GetComponent<T>().OnUpdate(); });
			}

			if constexpr (has_overridden_OnFixedUpdate<ComponentTemplate, T>::value)
			{
				OnFixedUpdateFunctions.emplace_back(typeid(T).name(), [&]() { this->GetComponent<T>().OnFixedUpdate(); });
			}

			return component;
		}

		template<typename T>
		T& GetComponent() 
		{
			CRESTA_ASSERT(!HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() 
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			CRESTA_ASSERT(!HasComponent<T>(), "Entity does not have component!");
			T& component = m_Scene->m_Registry.get<T>(m_EntityHandle);
			component.OnComponentRemoved();
			m_Scene->m_Registry.remove<T>(m_EntityHandle);

			OnStartFunctions.erase(
				std::remove_if(OnStartFunctions.begin(), OnStartFunctions.end(),
					[&](const auto& pair) { return pair.first == typeid(T).name(); }),
				OnStartFunctions.end());

			OnUpdateFunctions.erase(
				std::remove_if(OnUpdateFunctions.begin(), OnUpdateFunctions.end(),
					[&](const auto& pair) { return pair.first == typeid(T).name(); }),
				OnUpdateFunctions.end());

			OnFixedUpdateFunctions.erase(
				std::remove_if(OnFixedUpdateFunctions.begin(), OnFixedUpdateFunctions.end(),
					[&](const auto& pair) { return pair.first == typeid(T).name(); }),
				OnFixedUpdateFunctions.end());
		}

		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		UUID GetUUID() { return { GetComponent<IDComponent>().GetUUID() }; }
		inline void SetTag(const std::string& tag) { GetComponent<TagComponent>().Tag = tag; }
		inline const std::string& GetTage() { return GetComponent<TagComponent>().Tag; };

		inline bool IsValid() const { return m_EntityHandle != entt::null; }
		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
		
		void OnStart();
		void OnRender();
		void OnUpdate();
		void OnFixedUpdate();

	private:
		std::vector<std::pair<std::string, std::function<void()>>> OnStartFunctions;
		std::vector<std::pair<std::string, std::function<void()>>> OnRenderFunctions;
		std::vector<std::pair<std::string, std::function<void()>>> OnUpdateFunctions;
		std::vector<std::pair<std::string, std::function<void()>>> OnFixedUpdateFunctions;

		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
