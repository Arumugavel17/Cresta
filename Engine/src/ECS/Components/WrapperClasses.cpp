#include "WrapperClasses.hpp"
#include "Core/Application.hpp"
#include "ScriptComponent.hpp"

namespace Cresta
{
	EntityWrapper::EntityWrapper(Entity* entity)
	{
		this->p_Entity = entity;
	}

	TransformWrapper& EntityWrapper::GetTransform()
	{
		m_TransformWrapper.TransformComponent = &this->p_Entity->GetComponent<Transform>();
		return m_TransformWrapper;
	}

	void EntityWrapper::AttachMeshRenderer()
	{
		p_Entity->AddComponent<MeshRenderer>();
	}

	void EntityWrapper::AttachBoxCollider()
	{
		p_Entity->AddComponent<BoxCollider>();
	}

	void EntityWrapper::AttachScript(std::string path)
	{
		p_Entity->AddComponent<ScriptComponent>().SetPath(path);
	}

	SceneWrapper::SceneWrapper()
	{
		m_Scene = Application::GetApplication().GetActiveScene();
	}

	EntityWrapper SceneWrapper::CreateEntity()
	{
		m_Scene = Application::GetApplication().GetActiveScene();
		EntityWrapper Entity(m_Scene->CreateEntity("EmtityEntity").get());

		return Entity;
	}

	bool SceneWrapper::DestroyEntity(EntityWrapper entity)
	{
		m_Scene = Application::GetApplication().GetActiveScene();
		m_Scene->DestroyEntity(entity.p_Entity);
		return false;
	}
}