#include "EditorLayer.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Renderer.hpp"
#include "Core/Input.hpp"
#include "Core/ModelLoading/ModelLoader.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Cresta 
{
    EditorLayer::EditorLayer() : Layer("Editor Layer")
    {
        //m_Model = CreateRef<ModelLoader>("assets/models/backpack/backpack.obj");
        m_EditorCamera = new EditorCamera();
        
        m_ActiveScene = CreateRef<Scene>();
        m_HierarchyPanel = CreateRef<SceneHierarchyPanel>(m_ActiveScene);

        m_GridVertexArray = VertexArray::Create();
        m_GridShader = Shader::Create("assets/shaders/GridShader.glsl");
    }

    void EditorLayer::OnAttach()
	{
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1920;
        fbSpec.Height = 1080;
        m_Framebuffer = Framebuffer::Create(fbSpec);
    }

    void EditorLayer::OnUpdate()
    {
        if (m_SceneActive)
        {
            if (Input::GetKeyDown(Key::Escape))
            {
                ImGui::SetWindowFocus(nullptr);
            }
        }

        FramebufferSpecification spec = m_Framebuffer->GetSpecification();
        if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f 
            && 
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_EditorCamera->SetCameraWidthHeight(m_ViewportSize.x, m_ViewportSize.y);
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        Renderer::BeginScene((Camera)*m_EditorCamera);
        {
            m_EditorCamera->OnUpdate();
            m_Framebuffer->Bind();
            {
                RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
                RenderCommand::Clear();
                
                Renderer::DrawTriangle(m_GridShader,m_GridVertexArray,NULL,6);       
                m_ActiveScene->RenderScene();
                
                m_Framebuffer->Unbind();
            }
       
            Renderer::EndScene();
        }

    }

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnEvent(Event& e)
	{
        m_EditorCamera->OnEvent(e);
	}

    void EditorLayer::SetupDockSpace()
    {
        ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

        // Set the window flags for the main window
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // Main docking window
        ImGui::Begin("DockSpace Demo", nullptr, windowFlags);
        ImGuiID dockSpaceID = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), dockSpaceFlags);
        ImGui::End();

        // Remove any existing layout
        ImGui::DockBuilderRemoveNode(dockSpaceID); // Clear existing layout
        ImGui::DockBuilderAddNode(dockSpaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockSpaceID, ImGui::GetMainViewport()->WorkSize);

        // Split the dock space into regions
        ImGuiID dockLeft = ImGui::DockBuilderSplitNode(dockSpaceID, ImGuiDir_Left, 0.25f, nullptr, &dockSpaceID);
        ImGuiID dockRight = ImGui::DockBuilderSplitNode(dockSpaceID, ImGuiDir_Right, 0.25f, nullptr, &dockSpaceID);
        ImGuiID dockBottom = ImGui::DockBuilderSplitNode(dockSpaceID, ImGuiDir_Down, 0.25f, nullptr, &dockSpaceID);
        ImGuiID dockUp = ImGui::DockBuilderSplitNode(dockSpaceID, ImGuiDir_Up, 0.25f, nullptr, &dockSpaceID);
        
        // Dock windows to regions
        ImGui::DockBuilderDockWindow("Hierarchy", dockLeft);
        ImGui::DockBuilderDockWindow("Components", dockRight);
        ImGui::DockBuilderDockWindow("Project", dockBottom);
        ImGui::DockBuilderDockWindow("Scene", dockUp);

        // Complete the layout
        ImGui::DockBuilderFinish(dockSpaceID);
    }

    void EditorLayer::CreateDockSpace()
    {
        ImVec4 customColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // RGBA

        // Store the previous color to restore later
        ImVec4 previousColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

        // Set the custom color for this particular window
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = customColor;

        //SetupDockSpace();
        bool dockSpaceOpen = true;
        ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

        // Set the window flags for the main window
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // Main docking window
        ImGui::Begin("DockSpace Demo", &dockSpaceOpen, windowFlags);
        ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), dockSpaceFlags);
        ImGui::End();

    }

    void EditorLayer::OnImGUIRender()
    {
        CreateDockSpace();
        ShowScene();
        m_HierarchyPanel->OnImGuiRender();
    }
    
    void EditorLayer::ShowScene()
    {
        ImGui::Begin("Scene");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        m_SceneActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);
        m_EditorCamera->SetCameraMovementEnabled(m_SceneActive);

        ImGui::Image(textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
    }

}