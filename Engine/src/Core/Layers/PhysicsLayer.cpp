#include "PhysicsLayer.hpp"
#include "Scene/Components.hpp"

namespace Cresta
{
	PhysicsLayer::PhysicsLayer(Ref<Scene> scene) : Layer("Physics Layer",scene)
	{
		m_Physics = CreateRef<Physics>();
		Init();
	}

	PhysicsLayer ::~PhysicsLayer()
	{

	}

	void PhysicsLayer::Init()
	{
	}

	void PhysicsLayer::OnEvent(Event& e)
	{
	}

	void PhysicsLayer::SceneCallBack()
	{
	}

	void PhysicsLayer::OnFixedUpdate()
	{
		m_RigidbodyView = &m_ActiveScene->m_Registry.view<TransformComponent, Rigidbody>();
		if (m_RigidbodyView && m_RigidbodyView->size() > 0) 
		{
			m_Physics->Run();

			for (auto entity : *m_RigidbodyView)
			{
				auto& component = m_RigidbodyView->get<TransformComponent>(entity);
				component.Translation.x = m_Physics->position.GetX();
				component.Translation.y = m_Physics->position.GetY();
				component.Translation.z = m_Physics->position.GetZ();
			}
		}
	}

	void PhysicsLayer::OnUpdate()
	{

	}

	void PhysicsLayer::OnAttach()
	{
	}

	void PhysicsLayer::OnDetach()
	{
	}

	void PhysicsLayer::OnImGUIRender()
	{
	}
}