#include "Core/Application.hpp"
#include <GLFW/glfw3.h>
#include "Crestaph.hpp"

int main() {
	Cresta::Application* App = Cresta::CreateApplication();

	App->Run();
	return 0;
}
