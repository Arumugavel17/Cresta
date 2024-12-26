#include <iostream>
#include "spdlog/spdlog.h"
#include "Core/Application.hpp"
#include <cstdarg>
#include <thread>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <Jolt/Jolt.h>

int main() {
	Cresta::Application* App = Cresta::CreateApplication();
	App->Run();
	std::cout << "Hello world";
	if (glfwInit()) {
		std::cout << "j";
		spdlog::info("Welcome to spdlog!");
	}
	else {
		std::cout << "l";
	}
	int i = 0;
	std::cin >> i;
	return 0;
}
