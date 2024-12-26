#pragma once
#include <Crestaph.hpp>

namespace Cresta {
	class Layer {
	public:
		Layer();
		virtual ~Layer() = default;
		virtual void OnUpdate() = 0;
		virtual void OnEvent() = 0; 
		virtual void OnImGUIRender() = 0;
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual std::string toString() { return LayerName; }
	private:
		std::string LayerName;
	};
}