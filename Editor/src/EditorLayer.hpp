#pragma once 
#include "Core/Layer.hpp"

namespace Cresta {
	class EditorLayer : public Layer {
	public:
		virtual void OnEvent();
		virtual void OnEvent(Event& e);
		
		virtual void OnUpdate();
		virtual void OnAttach();
		virtual void OnDetach();
		
		virtual void OnImGUIRender();

		void Begin();
		void End();

		void SetDarkThemeColors();
	
	private:	
		bool m_BlockEvents = true;
	};
}