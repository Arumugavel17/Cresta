#pragma once
#include "Crestaph.hpp"
#include "Core/KeyCode.hpp"
#include "Core/MouseCode.hpp"

namespace Cresta 
{
	class Input 
	{
	public:
		static Input* Create();

		static bool GetKeyDown(KeyCode key);
		static bool GetKeyUp(KeyCode key);

		static bool GetMouseDown(MouseCode Code);
		static bool GetMouseUp(MouseCode Code);

		static std::pair<float,float> GetMousePosition();

		virtual bool GetKeyDownImpl(KeyCode key) = 0;
		virtual bool GetKeyUpImpl(KeyCode key) = 0;

		virtual bool GetMouseDownImpl(MouseCode Code) = 0;
		virtual bool GetMouseUpImpl(MouseCode Code) = 0;

		virtual std::pair<float, float> GetMousePositionImp() = 0;
	private:
		static Input* sm_InputClass;
	};
}

