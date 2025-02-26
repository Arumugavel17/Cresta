#include "Entity.hpp"

namespace Cresta 
{
	void Entity::OnUpdate()
	{
		for (auto i : OnUpdateFunctions)
		{
			i.second();
		}
	}
}