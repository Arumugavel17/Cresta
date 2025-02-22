#include "Core/Application.hpp"
#include "Crestaph.hpp"
#include "Debug/Profiler.hpp"
#include "ECS/Components/ScriptComponent.hpp"

int main() 
{
	auto temp = new Cresta::ScriptComponent("C:\\dev\\CrestaProjectFolder\\test.lua");

	temp->OnStart();
	temp->OnUpdate(50);
	temp->OnFixedUpdate();
	temp->OnValidate();
	temp->OnEnd();

	CRESTA_PROFILE_BEGIN_SESSION("app","C:\\dev\\Cresta\\results.json");
	Cresta::Application* App = Cresta::CreateApplication();
	App->Run();
	delete App;

	CRESTA_PROFILE_END_SESSION();
	return 0;
}
