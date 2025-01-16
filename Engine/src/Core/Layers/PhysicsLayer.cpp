#include "PhysicsLayer.hpp"

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