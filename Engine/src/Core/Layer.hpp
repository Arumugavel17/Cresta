#pragma once
#include "Crestaph.hpp"
#include "Core/Events/Event.hpp"

namespace Cresta {
	class Layer {
	public:
		Layer(std::string layername) : m_LayerName(layername){}
		virtual ~Layer() = default;

		virtual void OnEvent(Event& e) = 0;

		virtual void OnUpdate() = 0;
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual void OnImGUIRender() = 0;
		
		virtual std::string toString() { return m_LayerName; }
	private:
		std::string m_LayerName;
	};
}