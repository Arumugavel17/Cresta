#include "Entity.hpp"
#include "Entity.hpp"

namespace Cresta 
{
	void Entity::OnStart()
	{
		for (auto StartFunction : OnStartFunctions)
		{
			StartFunction.second();
		}
	}

	void Entity::OnRender()
	{
		for (auto RenderFunction : OnRenderFunctions)
		{
			RenderFunction.second();
		}
	}

	void Entity::OnUpdate()
	{
		for (auto UpdateFunction : OnUpdateFunctions)
		{
			UpdateFunction.second();
		}
	}

	void Entity::OnFixedUpdate()
	{
		for (auto FixedUpdateFunction : OnFixedUpdateFunctions)
		{
			FixedUpdateFunction.second();
		}
	}
}