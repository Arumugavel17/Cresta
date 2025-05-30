#pragma once 
#include "Core/Layers/Layer.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <ImGuizmo/ImGuizmo.h>

namespace Cresta 
{
	class ImGUILayer : public Layer 
	{
	public:
		ImGUILayer(Ref<Scene> scene) 
			: Layer("ImGUI Layer",scene)
		{
		}

		virtual ~ImGUILayer() = default ;
		virtual void OnEvent(Event& e) override;
		
		virtual void SceneCallBack() override;
		virtual void OnFixedUpdate() override;
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