#pragma once
#include "Core/Input.hpp"
#include "Core/KeyCode.hpp"

namespace Cresta 
{
	class OpenGLInput : public Input 
	{
	protected:
		bool GetKeyDownImpl(KeyCode key);
		bool GetKeyUpImpl(KeyCode key);

		bool GetMouseDownImpl(MouseCode Code);
		bool GetMouseUpImpl(MouseCode Code);

		std::pair<float, float> GetMousePositionImp();
	private:
		std::map<KeyCode, int> m_KeyState;
	};
}