#include "EditorLayer.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>      // Internal functions (required for DockBuilder APIs)
#include <imgui/imgui_impl_glfw.h>   // Platform-specific (GLFW example)
#include <imgui/imgui_impl_opengl3.h> // Renderer-specific (OpenGL example)
#include "Core/Application.hpp"
#include "Renderer/VertexArray.hpp"
#include "glad/glad.h"

namespace Cresta 
{
    EditorLayer::EditorLayer() : Layer("Editor Layer")
    {
        m_VertexArray = VertexArray::Create();
        m_GridVertexArray = VertexArray::Create();
        m_Shader = Shader::Create("C:/dev/Cresta/assets/shaders/FlatShader.glsl");
        m_GridShader = Shader::Create("C:/dev/Cresta/assets/shaders/GridShader.glsl");
        m_EditorCamera = new EditorCamera();

        for (int i = 0;i < 10;i++) {
            for (int j = 0;j < 10;j++) {
                glm::vec3 pos(i, 0, j);
                positions.push_back(pos);
            }
        }
    }

    void EditorLayer::OnAttach()
	{
        float vertices[] = {
            // Positions
            -0.5f, -0.5f, 0.0f, // Bottom-left
             0.5f, -0.5f, 0.0f, // Bottom-right
             0.5f,  0.5f, 0.0f, // Top-right
            -0.5f,  0.5f, 0.0f  // Top-left
        };

        unsigned int indices[] = {
            0, 1, 2, // First triangle
            2, 3, 0  // Second triangle
        };

        std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
        std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, 6);

        vertexBuffer->SetLayout({
                { ShaderDataType::FVec3 , "aPos" }
            });

        m_VertexArray->AddVertexBuffer(vertexBuffer);
        m_VertexArray->SetIndexBuffer(indexBuffer);

        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
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
        m_EditorCamera->OnUpdate();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();

        {
            m_GridShader->Bind();
            m_VertexArray->Bind();
            m_GridShader->SetMat4("model", glm::mat4(1.0f));
            m_GridShader->SetMat4("view", m_EditorCamera->GetViewMatrix());
            m_GridShader->SetMat4("projection", m_EditorCamera->GetProjectionMatrix());
            m_GridShader->SetVec3("cameraPos", m_EditorCamera->GetPosition());
            m_GridShader->SetVec3("gCameraWorldPos", m_EditorCamera->GetPosition());


            glDrawArrays(GL_TRIANGLES, 0, 6);
            m_GridVertexArray->Unbind();
            m_GridShader->Unbind();
        }

        {
            m_Shader->Bind();
            m_VertexArray->Bind();
            m_Shader->SetVec4("u_Color", glm::vec4(m_EditorCamera->GetCameraFront(),1.0f));
            m_Shader->SetMat4("u_View", m_EditorCamera->GetViewMatrix());
            m_Shader->SetMat4("u_Projection", m_EditorCamera->GetProjectionMatrix());

            for (int i = 0;i < 100;i++) 
            {
                m_Shader->SetMat4("u_Model", glm::translate(glm::mat4(1.0f),positions[i]));
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
            m_Shader->Unbind();
            m_VertexArray->Unbind();
        }

       

        m_Framebuffer->Unbind();
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

        // Dock windows to regions
        ImGui::DockBuilderDockWindow("Hierarchy", dockLeft);
        ImGui::DockBuilderDockWindow("Components", dockRight);
        ImGui::DockBuilderDockWindow("Project", dockBottom);

        // Complete the layout
        ImGui::DockBuilderFinish(dockSpaceID);
    }

    void EditorLayer::OnImGUIRender()
    {
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

        // Render docked 

        ImGui::Begin("Inspector");
        ImGui::Text("This is docked on the right.");
        ImGui::End();

        ImGui::Begin("Project");
        ImGui::Text("This is docked at the bottom.");
        ImGui::End();

        ImGui::Begin("Hierarchy");
        ImGui::Text("This is docked at the bottom.");
        ImGui::End();

        // Render texture in Window 1
        ImGui::Begin("Scene");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        glm::vec2 m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        
        m_SceneActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);
        m_EditorCamera->SetCameraMovementEnabled(m_SceneActive);

        ImGui::Image(textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
    }
}