#include "EditorLayer.hpp"
#include "Core/Input.hpp"
#include "Core/Application.hpp"

#include "Renderer/VertexArray.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Model.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Cresta 
{
    EditorLayer::EditorLayer(Ref<Scene> scene) : Layer("Editor Layer",scene)
    {
        m_EditorCamera = CreateRef<EditorCamera>();
        
        m_ActiveScene = scene;
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

    void EditorLayer::SceneCallBack()
    {
    }

    void EditorLayer::OnFixedUpdate()
    {
    }

    void EditorLayer::OnUpdate()
    {
        if (m_SceneActive)
        {
            if (Input::GetKeyDown(Key::Escape))
            {
                ImGui::SetWindowFocus(nullptr);
                m_SceneActive = false;
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
                m_ActiveScene->RenderScene();
                
                Renderer::DrawTriangle(m_GridShader, m_GridVertexArray, NULL, 6);
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
        
        ShowFileManager("assets","assets");

        ShowScene();
        m_HierarchyPanel->OnImGuiRender();

        ImGui::Begin("VSync");
        if (ImGui::Checkbox("Set VSync Option", &m_VSync))
        {
            // Logic when the state of the checkbox changes
            Application::GetApplication().GetWindow()->SetVSync(m_VSync);
            if (m_VSync)
            {
                CRESTA_CORE_INFO("Enabled VSync");
            }
            else
            {
                CRESTA_CORE_INFO("Disabled VSync");
            }
        }
        ImGui::End();
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

    void EditorLayer::ShowFileManager(const std::filesystem::path& directory, const std::string& currentPath)
    {
        ImGui::Begin("File Manager");
        ImGui::Text("File Manager - %s", currentPath.c_str());

        // List directories first
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            std::string filename = entry.path().filename().string();
            if (entry.is_directory())
            {
                if (ImGui::TreeNode(entry.path().filename().string().c_str()))
                {
                    ShowFileManager(entry.path(), currentPath + "/" + entry.path().filename().string());
                    ImGui::TreePop();
                }
            }
            else if (entry.is_regular_file())
            {
                if (ImGui::Selectable(entry.path().filename().string().c_str()))
                {
                    //TODO: Create a way open files through the editor
                }

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoPreviewTooltip))
                {
                    std::string payload = currentPath + "/" + filename;
                    // Set the drag payload to the file path
                    ImGui::SetDragDropPayload("FILE_PATH", payload.c_str(), payload.length() + 1);
                    ImGui::Text(filename.c_str()); // Display file name as label 
                    ImGui::EndDragDropSource();
                }
            }

        }

        ImGui::End();
    }
}