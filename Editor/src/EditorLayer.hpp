#pragma once
#include "Core/Layers/Layer.hpp"
#include "EditorCamera.hpp"

#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "Renderer/Model.hpp"

#include "ECS/Scene/Scene.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene/SceneHierarchyPanel.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>      
#include <imgui/imgui_impl_glfw.h>   
#include <imgui/imgui_impl_opengl3.h>


namespace Editor
{
	using namespace Cresta;
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(Cresta::Ref<Scene> scene);

		virtual void SceneCallBack() override;
		virtual void OnFixedUpdate() override;

		virtual void OnUpdate() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		
		virtual void OnEvent(Event& e) override;
		virtual void OnImGUIRender() override;

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		
		void ShowFileManager(const std::filesystem::path& directory, const std::string& currentPath);
		void SetupDockSpace();

		void NewScene();
		void OpenScene();
		void SaveScene();

		void SaveProject();
		void NewProject();
		void OpenProject();

		void OnScenePlay();
		void OnSceneSimulate();
		void OnSceneStop();
		void OnScenePause();

		void OnDuplicateEntity();

		// UI Panels
		void UI_Toolbar();
	private:
		void CreateDockSpace();
		void ShowScene();

	private:
		enum class EditorState
		{
			Edit = 0, Play = 1, Simulate = 2
		};
		EditorState m_EditorState = EditorState::Edit;

		bool m_VSync = true;
		ImVec2 previousWindowSize = ImVec2(0, 0);
		
		Cresta::Ref<SceneHierarchyPanel> m_HierarchyPanel;
		Cresta::Ref<EditorCamera> m_EditorCamera;

		int m_EntityID = -1;

		Cresta::Ref<Framebuffer> m_Framebuffer;
		int m_GizmoType = -1;

		bool StopEditor = false;

		Cresta::Ref<Shader> m_GridShader;
		Cresta::Ref<VertexArray> m_GridVertexArray;

		ImVec2 m_ViewportSize;
		bool m_SceneActive = false;

		ImVec2 m_ViewportBounds[2];

		Cresta::Ref<Texture2D> m_IconPlay, m_IconStop, m_NotEmptyFolder, m_EmptyFolder;

		float m_MouseX, m_MouseY;
	};
}