#pragma once
#include "Crestaph.hpp"

namespace Cresta
{
	class Dispatcher
	{
	public:
		void Subscribe(const std::string& key, std::function<void(bool)> function)
		{
			Observers[key] = function;
		}

		void UnSubscribe(const std::string& key)
		{
			Observers.erase(key);
		}

		void post(bool value) const
		{
			for (auto& observer : Observers)
			{
				observer.second(value);
			}
		}

	private:
		std::map<std::string, std::function<void(bool)>> Observers;
	};
}
