#include "EditorLayer.hpp"
#include "EditorLayer.hpp"

#include "Core/Input.hpp"
#include "Core/Application.hpp"

#include "Renderer/VertexArray.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Model.hpp"

#include <imgui/imgui.h>
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
        m_IconPlay = Texture2D::Create("assets/Icons/PlayButton.png");
        m_IconPause = Texture2D::Create("assets/Icons/PauseButton.png");
        m_IconSimulate = Texture2D::Create("assets/Icons/SimulateButton.png");
        m_IconStep = Texture2D::Create("assets/Icons/StepButton.png");
        m_IconStop = Texture2D::Create("assets/Icons/StopButton.png");

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
        m_ActiveScene->FixedUpate();
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
        // Begin DockSpace
        ImGui::Begin("DockSpace Demo", &dockSpaceOpen, windowFlags);

        if (Input::GetKeyDown(Key::LeftControl) && Input::GetKeyDown(Key::O))
        {
            OpenScene();
        }
        if (Input::GetKeyDown(Key::LeftControl) && Input::GetKeyDown(Key::S))
        {
            SaveScene(); 
        }
        // Add a menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {

                if (ImGui::MenuItem("New Scene"))
                {
                    NewScene();
                }
                if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
                {
                    OpenScene();
                }
                if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
                {
                    SaveScene();
                }
            
                if (ImGui::MenuItem("Exit"))
                {
                    Application::GetApplication().Close();
                }
                ImGui::EndMenu();
            }


            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) 
                { 
                    /* Handle Undo action */ 
                }
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) 
                { 
                    /* Handle Redo action */ 
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About")) 
                { 
                    /* Handle About action */ 
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Add the dock space
        ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), dockSpaceFlags);

        // End DockSpace
        ImGui::End();

    }

    void EditorLayer::OnImGUIRender()
    {
        CreateDockSpace();
        UI_Toolbar();
        ShowFileManager("assets", "assets");
        ShowScene();
        m_HierarchyPanel->OnImGuiRender();

        // VSync settings
        ImGui::Begin("VSync");
        if (ImGui::Checkbox("Set VSync Option", &m_VSync)) {
            Application::GetApplication().GetWindow()->SetVSync(m_VSync);
            if (m_VSync) {
                CRESTA_CORE_INFO("Enabled VSync");
            }
            else {
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

    void EditorLayer::UI_Toolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
        ImGui::Begin("toolbar" , 0 , ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
        ImGui::BeginChild("##tools",ImVec2(0,0),ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);

        // Bottom padding value
        float bottomPadding = 10.0f;

        bool toolbarEnabled = (bool)m_ActiveScene;
        ImVec4 tintColor = ImVec4(1, 1, 1, 1);
        if (!toolbarEnabled)
            tintColor.w = 0.5f;

        // Button size
        float buttonSize = ImGui::GetWindowHeight() - 4.0f - bottomPadding; // Adjusted for padding

        // Total button count (depending on conditions)
        int buttonCount = 0;
        if (m_EditorState == EditorState::Edit || m_EditorState == EditorState::Play)
        {
            buttonCount++; // Play button
        }
        if (m_EditorState == EditorState::Edit || m_EditorState == EditorState::Simulate)
        {
            buttonCount++; // Simulate button
        }
        if (m_EditorState != EditorState::Edit)
        {
            buttonCount++; // Pause button
        }
        if (m_ActiveScene->IsPaused())
        {
            buttonCount++; // Step button
        }

        // Calculate spacing and center offset
        float windowWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
        float totalWidth = (buttonSize + ImGui::GetStyle().ItemSpacing.x) * buttonCount - ImGui::GetStyle().ItemSpacing.x;
        float startX = (windowWidth - totalWidth) / 2.0f;

        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x + startX);

        Ref<Texture2D> icon = (m_EditorState == EditorState::Edit || m_EditorState == EditorState::Simulate) ? m_IconPlay : m_IconStop;
        if (ImGui::ImageButton("PlayButton", (ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
        {
            if (m_EditorState == EditorState::Edit)
            {
                OnScenePlay();
            }
            else if (m_EditorState == EditorState::Play)
            {
                OnSceneStop();
            }
        }

        // Pause button
        if (m_EditorState != EditorState::Edit)
        {
            bool isPaused = m_ActiveScene->IsPaused();
            Ref<Texture2D> icon = m_IconPause;
            if (ImGui::ImageButton("Pause", (ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
            {
                m_ActiveScene->SetPaused(!isPaused);
            }
            ImGui::SameLine();
        }

        // Add bottom padding
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + bottomPadding);

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);

        ImGui::EndChild();
        ImGui::End();

    }

    void EditorLayer::NewScene()
    {
        Application::GetApplication().NewScene();
        m_HierarchyPanel->SetScene(m_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
        Application::GetApplication().OpenScene();
        m_HierarchyPanel->SetScene(m_ActiveScene); 
    }

    void EditorLayer::SaveScene()
    {
        Application::GetApplication().SaveScene();
    }

    void EditorLayer::OnScenePlay()
    {
        CRESTA_CORE_INFO("OnScenePlay");
        m_EditorState = EditorState::Play;
        m_ActiveScene->OnRuntimeStart();
    }

    void EditorLayer::OnSceneSimulate()
    {
        CRESTA_CORE_INFO("OnSceneSimulate");
    }

    void EditorLayer::OnSceneStop()
    {
        CRESTA_CORE_INFO("OnSceneStop");
        m_EditorState = EditorState::Edit;
        m_ActiveScene->OnRuntimeStop();
    }

    void EditorLayer::OnScenePause()
    {
        CRESTA_CORE_INFO("OnScenePause");
    }

    void EditorLayer::OnDuplicateEntity()
    {
    }
}