#pragma once
#include "Core/Input.hpp"

namespace Cresta {
	class OpenGLInput : public Input {
	public:
		bool GetKeyDownImpl(KeyCode key);
		bool GetKeyUpImpl(KeyCode key);

		bool GetMouseDownImpl(MouseCode Code);
		bool GetMouseUpImpl(MouseCode Code);

		std::pair<float, float> GetMousePositionImp();
	};
}