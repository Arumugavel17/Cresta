#pragma once
#include "Core/Layer.hpp"

namespace Cresta {
	class EditorLayer : public Layer{
	public:
		EditorLayer() : Layer("Editor Layer") {}

		virtual void OnUpdate() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& e) override;
		virtual void OnImGUIRender() override;

	private:

	};
}