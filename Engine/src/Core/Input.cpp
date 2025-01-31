#include "Core/Input.hpp"
#include "Platform/OpenGL/OpenGLInput.hpp"

namespace Cresta 
{
	Input* Input::sm_InputClass = Input::Create();

	Input* Input::Create()
	{
		return new OpenGLInput();
	}

	bool Input::GetKeyDown(KeyCode key)
	{
		return sm_InputClass->GetKeyDownImpl(key);
	}

	bool Input::GetKeyUp(KeyCode key)
	{
		return sm_InputClass->GetKeyUpImpl(key);
	}

	bool Input::GetMouseDown(MouseCode Code)
	{
		return sm_InputClass->GetMouseDownImpl(Code);
	}

	bool Input::GetMouseUp(MouseCode Code)
	{
		return sm_InputClass->GetMouseUp(Code);
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		return sm_InputClass->GetMousePositionImp();
	}
}