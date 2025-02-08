#include "Core/Application.hpp"
#include "Crestaph.hpp"
#include "Debug/Profiler.hpp"
#include "ScriptEngine/LuaManager.hpp"

int main() 
{
	Cresta::LuaManager L;
	L.RunScript("C:/dev/CrestaProjectFolder/test.lua");
	int result = L.callLuaFunction("add", 5, 3);
	std::cout << "Lua returned: " << result << std::endl;

	CRESTA_PROFILE_BEGIN_SESSION("app","C:\\dev\\Cresta\\results.json");
	Cresta::Application* App = Cresta::CreateApplication();
	App->Run();
	delete App;

	CRESTA_PROFILE_END_SESSION();
	return 0;
}
