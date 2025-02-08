#pragma once
#include "Crestaph.hpp"

namespace Cresta
{
	extern "C" 
	{
		#include "lua.hpp"
	}
	class LuaManager
	{
	public:
		LuaManager();
		~LuaManager();

		void RunScript(const std::string& script);

		int callLuaFunction(const std::string& function, int a, int b);
	private:
		lua_State* L;
	};
}