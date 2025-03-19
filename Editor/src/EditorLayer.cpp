#include "EditorLayer.hpp"

#include "Core/Input.hpp"
#include "Core/Application.hpp"

#include "Renderer/VertexArray.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Model.hpp"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Editor
{
    Cresta::Ref<Cresta::VertexArray> m_PrimitiveCube;
    Cresta::Ref<Cresta::Shader> m_Shader;

    EditorLayer::EditorLayer(Cresta::Ref<Scene> scene) : Layer("Editor Layer",scene) ,m_MouseX(0), m_MouseY(0)
    {
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

        m_EditorCamera = CreateRef<EditorCamera>();
        
        p_ActiveScene = scene;
        
        m_HierarchyPanel = CreateRef<SceneHierarchyPanel>(p_ActiveScene);
        m_HierarchyPanel->SetEditorCamera(m_EditorCamera);

        m_GridVertexArray = VertexArray::Create();
        m_GridShader = Shader::Create("assets/shaders/GridShader.glsl");
    }

    void EditorLayer::OnAttach()
	{
        CRESTA_PROFILE_FUNCTION();
        m_IconPlay = Texture2D::Create("assets/Icons/PlayButton.png");
        m_IconStop = Texture2D::Create("assets/Icons/StopButton.png");
        m_EmptyFolder = Texture2D::Create("assets/Icons/EmptyFolder.png");
        m_NotEmptyFolder = Texture2D::Create("assets/Icons/NotEmptyFolder.png");

        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { 
            FramebufferTextureFormat::RGBA8,
            FramebufferTextureFormat::RED_INTEGER,
            FramebufferTextureFormat::Depth 
        };
        fbSpec.Width = 1920;
        fbSpec.Height = 1080;
        m_Framebuffer = Framebuffer::Create(fbSpec);
    }

    void EditorLayer::SceneCallBack()
    {
    }

    void EditorLayer::OnFixedUpdate()
    {
        CRESTA_PROFILE_FUNCTION();
        p_ActiveScene->FixedUpate();
    }

    void EditorLayer::OnUpdate()
    {
        CRESTA_PROFILE_FUNCTION();
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

        Renderer::BeginScene(*m_EditorCamera);
        {
            m_EditorCamera->OnUpdate();
            m_Framebuffer->Bind();
            {
                p_ActiveScene->OnUpdate();
                Renderer::DrawTriangle(m_GridShader, m_GridVertexArray, NULL, 6);

                m_EntityID = m_Framebuffer->ReadPixel(1, m_MouseX, m_MouseY);
                m_Framebuffer->Unbind();
            }
            Renderer::EndScene();
        }
    }


    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
    {
        CRESTA_PROFILE_FUNCTION();
        using namespace glm;
        using T = float;

        mat4 LocalMatrix(transform);

        // Normalize the matrix.
        if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
            return false;

        // First, isolate perspective.  This is the messiest.
        if (
            epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
        {
            // Clear the perspective partition
            LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
            LocalMatrix[3][3] = static_cast<T>(1);
        }

        // Next take care of translation (easy).
        translation = vec3(LocalMatrix[3]);
        LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

        vec3 Row[3], Pdum3;

        // Now get scale and shear.
        for (length_t i = 0; i < 3; ++i)
            for (length_t j = 0; j < 3; ++j)
                Row[i][j] = LocalMatrix[i][j];

        // Compute X scale factor and normalize first row.
        scale.x = length(Row[0]);
        Row[0] = detail::scale(Row[0], static_cast<T>(1));
        scale.y = length(Row[1]);
        Row[1] = detail::scale(Row[1], static_cast<T>(1));
        scale.z = length(Row[2]);
        Row[2] = detail::scale(Row[2], static_cast<T>(1));

        // At this point, the matrix (in rows[]) is orthonormal.
        // Check for a coordinate system flip.  If the determinant
        // is -1, then negate the matrix and the scaling factors.
#if 0
        Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
        if (dot(Row[0], Pdum3) < 0)
        {
            for (length_t i = 0; i < 3; i++)
            {
                scale[i] *= static_cast<T>(-1);
                Row[i] *= static_cast<T>(-1);
            }
        }
#endif

        rotation.y = asin(-Row[0][2]);
        if (cos(rotation.y) != 0) {
            rotation.x = atan2(Row[1][2], Row[2][2]);
            rotation.z = atan2(Row[0][1], Row[0][0]);
        }
        else {
            rotation.x = atan2(-Row[2][0], Row[1][1]);
            rotation.z = 0;
        }


        return true;
    }

    void EditorLayer::ShowScene()
    {
        CRESTA_PROFILE_FUNCTION();
        ImGui::Begin("Scene");

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();

        m_ViewportSize = {
            viewportPanelSize.x,
            viewportPanelSize.y };

        auto mousePos = Input::GetMousePosition();
        auto [mouseX, mouseY] = mousePos;

        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();

        m_MouseX = mouseX - windowPos.x;
        m_MouseY = mouseY - windowPos.y;

        m_SceneActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);
        m_EditorCamera->SetCameraMovementEnabled(m_SceneActive);

        ImGui::Image(textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
        ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        ImVec2 viewportOffset = ImGui::GetWindowPos();
        ImVec2 m_ViewportBoundsMin = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        ImVec2 m_ViewportBoundsMax = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        Entity* selectedEntity = m_HierarchyPanel->GetSelectedEntity();
        if (selectedEntity && selectedEntity->IsValid() && m_GizmoType != -1)
        {

            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(m_ViewportBoundsMin.x, m_ViewportBoundsMin.y, m_ViewportBoundsMax.x - m_ViewportBoundsMin.x, m_ViewportBoundsMax.y - m_ViewportBoundsMin.y);

            const glm::mat4& cameraProjection = m_EditorCamera->GetProjectionMatrix();
            glm::mat4 cameraView = m_EditorCamera->GetViewMatrix();

            auto& tc = selectedEntity->GetComponent<Transform>();
            glm::mat4 transform = tc.GetTransform();

            if (ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, nullptr))
            {
                glm::vec3 translation, rotation, scale;
                DecomposeTransform(transform, translation, rotation, scale);

                glm::quat deltaRotation = glm::quat(rotation) - tc.GetRotation();
                tc.SetPosition(translation);
                tc.SetRotation(tc.GetRotation() += deltaRotation);
                tc.SetScale(scale);
                tc.OnValidate.post(true);
            }
        }

        ImGui::End();
    }

	void EditorLayer::OnDetach()
	{
        CRESTA_PROFILE_FUNCTION();
	}

	void EditorLayer::OnEvent(Event& e)
	{
        CRESTA_PROFILE_FUNCTION();
        m_EditorCamera->OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(CRESTA_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<MouseButtonPressedEvent>(CRESTA_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

    bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        CRESTA_PROFILE_FUNCTION();
        if (!e.Has(Key::LeftShift) && e.Has(Key::LeftControl, Key::O))
        {
            OpenScene();
        }
        if (!e.Has(Key::LeftShift) && e.Has(Key::LeftControl,Key::S))
        {
            SaveScene();
        }

        if (e.Has(Key::LeftShift))
        {
            std::cout << "\x1B[2J\x1B[H"; // ANSI escape sequence
        }

        if (e.Has(Key::LeftControl,Key::LeftShift, Key::O))
        {
            OpenProject();
        }
        if (e.Has(Key::LeftControl, Key::LeftShift, Key::S))
        {
            SaveProject();
        }

        if (!e.Has(Key::LeftAlt) && !ImGuizmo::IsUsing())
        {
            if (e.Has(Key::Q))
            {
                m_GizmoType = -1;
            }
            if (e.Has(Key::W))
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            if (e.Has(Key::E))
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            if (e.Has(Key::R))
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            }
        }
        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        CRESTA_PROFILE_FUNCTION();
        if (e.GetMouseButton() == Mouse::ButtonLeft)
        {
            if (m_EntityID >= 0)
            {
                m_HierarchyPanel->SetSelectedEntity((entt::entity)m_EntityID);
                return true;
            }
        }
        return false;
    }

    void EditorLayer::CreateDockSpace()
    {
        CRESTA_PROFILE_FUNCTION();

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
        // Add a menu bar
        if (ImGui::BeginMenuBar())
        {
            Application::ProjectPath& ProjectDetails = Application::GetApplication().p_ActiveProjectPath;
            
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

                if (ImGui::MenuItem("New Project"))
                {
                    NewProject();
                }
                if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O"))
                {
                    OpenProject();
                }
                if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S"))
                {
                    SaveProject();
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
            
            ImGui::Text((ProjectDetails.ProjectFile + " " + ProjectDetails.ProjectFolder.string()).c_str());

            ImGui::EndMenuBar();
        }

        // Add the dock space
        ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), dockSpaceFlags);

        // End DockSpace
        ImGui::End();

    }

    void EditorLayer::OnImGUIRender()
    {
        CRESTA_PROFILE_FUNCTION();

        CreateDockSpace();
        UI_Toolbar();
        string RootPath = Application::GetApplication().p_ActiveProjectPath.ProjectFolder.string();

        ShowFileManager(RootPath, RootPath);
        
        ShowScene();
        m_HierarchyPanel->OnImGuiRender();

        // VSync settings
        ImGuizmo::BeginFrame();
        ImGui::Begin("VSync");
        if (ImGui::Checkbox("Set VSync Option", &m_VSync)) 
        {
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

    void EditorLayer::ShowFileManager(const std::filesystem::path& directory, const std::string& currentPath)
    {
        CRESTA_PROFILE_FUNCTION();

        ImGui::Begin("File Manager");
        ImGui::Text("File Manager - %s", currentPath.c_str());

        // List directories first
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            std::string filename = entry.path().filename().string();
            if (entry.is_directory())
            {
                ImGui::BeginGroup(); // Group image and text together
                if (ImGui::TreeNode(filename.c_str()))
                {
                    ShowFileManager(entry.path(), currentPath + "/" + filename);
                    ImGui::TreePop();
                }
                ImGui::EndGroup();
            }
            else if (entry.is_regular_file())
            {
                if (ImGui::Selectable(filename.c_str()))
                {
                    //TODO: Create a way open files through the editor
                }

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoPreviewTooltip))
                {
                    std::string payload = currentPath + "\\" + filename;
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
        CRESTA_PROFILE_FUNCTION();

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
        CRESTA_PROFILE_FUNCTION();

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

        bool toolbarEnabled = (bool)p_ActiveScene;
        ImVec4 tintColor = ImVec4(1, 1, 1, 1);
        if (!toolbarEnabled)
        {
            tintColor.w = 0.5f;
        }

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
        if (p_ActiveScene->IsPaused())
        {
            buttonCount++; // Step button
        }

        // Calculate spacing and center offset
        float windowWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
        float totalWidth = (buttonSize + ImGui::GetStyle().ItemSpacing.x) * buttonCount - ImGui::GetStyle().ItemSpacing.x;
        float startX = (windowWidth - totalWidth) / 2.0f;

        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x + startX);

        Cresta::Ref<Texture2D> icon = (m_EditorState == EditorState::Edit || m_EditorState == EditorState::Simulate) ? m_IconPlay : m_IconStop;
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
            bool isPaused = p_ActiveScene->IsPaused();
            Cresta::Ref<Texture2D> icon = m_IconStop;
            if (ImGui::ImageButton("Pause", (ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
            {
                p_ActiveScene->SetPaused(!isPaused);
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
        CRESTA_PROFILE_FUNCTION();

        Application::GetApplication().NewScene(); 
        //p_ActiveScene will be updated by Application through SetSceneForLayers
        m_HierarchyPanel->SetScene(p_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
        CRESTA_PROFILE_FUNCTION();

        Application::GetApplication().OpenScene();
        //p_ActiveScene will be updated by Application through SetSceneForLayers
        m_HierarchyPanel->SetScene(p_ActiveScene); 
    }

    void EditorLayer::SaveScene()
    {
        CRESTA_PROFILE_FUNCTION();

        Application::GetApplication().SaveScene();
    }

    void EditorLayer::SaveProject()
    {
        CRESTA_PROFILE_FUNCTION();

        Application::GetApplication().SaveProject();
    }

    void EditorLayer::NewProject()
    {
        CRESTA_PROFILE_FUNCTION();

        Application::GetApplication().NewProject();
    }

    void EditorLayer::OpenProject()
    {
        CRESTA_PROFILE_FUNCTION();

        Application::GetApplication().OpenProject();
    }

    void EditorLayer::OnScenePlay()
    {
        CRESTA_PROFILE_FUNCTION();

        CRESTA_CORE_INFO("OnScenePlay");
        m_EditorState = EditorState::Play;
        p_ActiveScene->OnRuntimeStart();
    }

    void EditorLayer::OnSceneSimulate()
    {
        CRESTA_PROFILE_FUNCTION();

        CRESTA_CORE_INFO("OnSceneSimulate");
    }

    void EditorLayer::OnSceneStop()
    {
        CRESTA_PROFILE_FUNCTION();

        CRESTA_CORE_INFO("OnSceneStop");
        m_EditorState = EditorState::Edit;
        p_ActiveScene->OnRuntimeStop();
    }

    void EditorLayer::OnScenePause()
    {
        CRESTA_PROFILE_FUNCTION();

        CRESTA_CORE_INFO("OnScenePause");
    }

    void EditorLayer::OnDuplicateEntity()
    {
        CRESTA_PROFILE_FUNCTION();
    }
}