#pragma once
#include "Core/Layers/Layer.hpp"
#include "EditorCamera.hpp"

#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "Renderer/Model.hpp"

#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"
#include "Scene/SceneHierarchyPanel.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>      
#include <imgui/imgui_impl_glfw.h>   
#include <imgui/imgui_impl_opengl3.h>

namespace Cresta {
	class EditorLayer : public Layer{
	public:
		EditorLayer(Ref<Scene> scene);

		virtual void SceneCallBack() override;
		virtual void OnFixedUpdate() override;
		virtual void OnUpdate() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGUIRender() override;
		
		void ShowFileManager(const std::filesystem::path& directory, const std::string& currentPath);

		void SetupDockSpace();

	private:
		void CreateDockSpace();
		void ShowScene();
	private:

		bool m_VSync = true;
		ImVec2 previousWindowSize = ImVec2(0, 0);
		
		Ref<Scene> m_ActiveScene;
		Ref<SceneHierarchyPanel> m_HierarchyPanel;
		
		Ref<Framebuffer> m_Framebuffer;
		
		Ref<Shader> m_GridShader;
		Ref<VertexArray> m_GridVertexArray;

		Ref<EditorCamera> m_EditorCamera;
		ImVec2 m_ViewportSize;
		bool m_SceneActive = false;
	};
}