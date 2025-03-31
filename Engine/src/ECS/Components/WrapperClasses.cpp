#include "WrapperClasses.hpp"
#include "Core/Application.hpp"

namespace Cresta
{
	EntityWrapper::EntityWrapper(Entity* entity) : p_Entity(entity)
	{
	}

	TransformWrapper& EntityWrapper::GetTransform()
	{
		//std::cout << "[C++] GetTransform() called!" << std::endl;
		m_TransformWrapper.TransformComponent = &p_Entity->GetComponent<Transform>();
		return m_TransformWrapper;
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
}