#include "Entity.hpp"
#include "Entity.hpp"
#include "Entity.hpp"

namespace Cresta 
{
	void Entity::OnStart()
	{
		CRESTA_PROFILE_FUNCTION();
		for (auto StartFunction : OnStartFunctions)
		{
			StartFunction.second();
		}
	}

	void Entity::OnRender()
	{
		CRESTA_PROFILE_FUNCTION();
		for (auto RenderFunction : OnRenderFunctions)
		{
			RenderFunction.second();
		}
	}

	void Entity::OnUpdate()
	{
		CRESTA_PROFILE_FUNCTION();
		for (auto UpdateFunction : OnUpdateFunctions)
		{
			UpdateFunction.second();
		}
	}

	void Entity::OnFixedUpdate()
	{
		CRESTA_PROFILE_FUNCTION();
		for (auto FixedUpdateFunction : OnFixedUpdateFunctions)
		{
			FixedUpdateFunction.second();
		}
	}

	void Entity::OnEnd()
	{
		CRESTA_PROFILE_FUNCTION();
		for (auto RenderFunction : OnEndFunctions)
		{
			RenderFunction.second();
		}
	}
}