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
#include "Scene/ContentBrowserPanel.hpp"

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

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

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
		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};
		SceneState m_SceneState = SceneState::Edit;

		bool m_VSync = true;
		ImVec2 previousWindowSize = ImVec2(0, 0);
		
		Ref<Scene> m_EditorScene;
		Ref<Scene> m_ActiveScene;
		
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;
		Ref<SceneHierarchyPanel> m_HierarchyPanel;

		Ref<Framebuffer> m_Framebuffer;
		
		Ref<Shader> m_GridShader;
		Ref<VertexArray> m_GridVertexArray;

		Ref<EditorCamera> m_EditorCamera;
		ImVec2 m_ViewportSize;
		bool m_SceneActive = false;

		std::filesystem::path m_EditorScenePath;
		Ref<Texture2D> m_IconPlay, m_IconPause, m_IconStep, m_IconSimulate, m_IconStop;
	};
}