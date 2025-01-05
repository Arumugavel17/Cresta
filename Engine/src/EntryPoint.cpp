#include "Core/Application.hpp"
#include "Crestaph.hpp"

int main() {
	Cresta::Application* App = Cresta::CreateApplication();

	App->Run();
	return 0;
}
