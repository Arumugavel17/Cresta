#pragma once
#include "Crestaph.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Editor
{
	class ProjectSelection
	{
	public:
		ProjectSelection();
		~ProjectSelection();
		void Run();
	private:
		void CreateProjectSelectionWindow();
	};
}