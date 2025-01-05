#pragma once 
#include "Core/Layer.hpp"

namespace Cresta {
	class ImGUILayer : public Layer 
	{
	public:

		ImGUILayer() : Layer("ImGUI Layer"){}
		virtual ~ImGUILayer() = default ;
		virtual void OnEvent(Event& e) override;
		
		virtual void OnUpdate() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		
		virtual void OnImGUIRender() override;

		void Begin();
		void End();

		void SetDarkThemeColors();
		uint32_t GetActiveWidgetID() const;
	private:	
		bool m_BlockEvents = true;
	};
}