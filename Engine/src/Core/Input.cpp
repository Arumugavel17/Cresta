#include "Core/Input.hpp"
#include "Platform/OpenGL/OpenGLInput.hpp"

namespace Cresta {
	Cresta::Input* Cresta::Input::s_InputClass = Input::Create();

	Input* Input::Create()
	{
		return new OpenGLInput();
	}

	bool Input::GetKeyDown(KeyCode key)
	{
		return s_InputClass->GetKeyDownImpl(key);
	}
	bool Input::GetKeyUp(KeyCode key)
	{
		return s_InputClass->GetKeyUpImpl(key);
	}
	bool Input::GetMouseDown(MouseCode Code)
	{
		return s_InputClass->GetMouseDownImpl(Code);
	}
	bool Input::GetMouseUp(MouseCode Code)
	{
		return s_InputClass->GetMouseUp(Code);
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		return s_InputClass->GetMousePositionImp();
	}
}