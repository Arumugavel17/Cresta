#include "Crestaph.hpp"
#include "EditorApplication.hpp"
#include "Platform/OpenGL/Utils.hpp"
#include "ToastMessage.hpp"

#include <nlohmann/json.hpp>

namespace Editor
{
	namespace fs = std::filesystem;
	using json = nlohmann::json;
	
	static void ReadProjectsList(std::map<int, std::pair<string, string>, std::greater<int>>& output)
	{
		std::string filePath = "assets/Projects.json";
		std::ifstream AlreadyOpenedProject(filePath);

		if (!AlreadyOpenedProject) 
		{
			// File does not exist, create it
			std::ofstream outFile(filePath);
			json j = json::object();  // Ensure it's a valid JSON object `{}` instead of `[]`
			outFile << j;
			outFile.close();

			AlreadyOpenedProject.open(filePath);  // Open the newly created file
		}

		// Check if the file is empty
		if (AlreadyOpenedProject.peek() == std::ifstream::traits_type::eof()) 
		{
			std::cout << "File is empty, skipping parsing.\n";
			AlreadyOpenedProject.close();
			return;
		}

		// Now safe to parse
		json JProject;
		AlreadyOpenedProject >> JProject;

		for (json::iterator it = JProject.begin(); it != JProject.end(); ++it) 
		{
			json::iterator ProjectData = it.value().begin();
			std::string ProjectPath = ProjectData.value().get<std::string>(); // Directly get string
			++ProjectData;

			output[ProjectData.value()] = { it.key(), ProjectPath };
		}

		AlreadyOpenedProject.close();
	}

	static void CreateOpenWindow()
	{
		ImVec4 customColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // RGBA
		ImVec4 previousColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = customColor;
		bool dockSpaceOpen = true;
		ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("OpenWindow", &dockSpaceOpen, windowFlags);
		ImGui::DockSpace(ImGui::GetID("OpenWindow"), ImVec2(0.0f, 0.0f), dockSpaceFlags);
		ImGui::End();
	}

	void EditorApplication::SaveProject()
	{
		std::string filepath = Utils::FileDialogs::SaveScene("Cresta Scene (*.cproj)\0*.cproj\0");
		if (!filepath.empty())
		{
			std::ofstream save(filepath);
			save.close();
			p_ActiveProjectPath.first = filepath.substr(filepath.find_last_of("/")+1,filepath.length());
			p_ActiveProjectPath.second = filepath;
		}
	}

	void EditorApplication::NewProject()
	{
		std::string filepath = Utils::FileDialogs::GetProjectFolder();
		if (!filepath.empty())
		{
			string strpath = filepath;
			p_ActiveProjectPath.first = strpath.substr(strpath.find_last_of("\\") + 1, strpath.length());
			p_ActiveProjectPath.second = strpath;
		}
	}

	void EditorApplication::OpenProject()
	{
	

		std::string filepath = Utils::FileDialogs::OpenProject();
		if (!filepath.empty())
		{
			string strpath = filepath;
			p_ActiveProjectPath.first = strpath.substr(strpath.find_last_of("\\") + 1, strpath.length());
			p_ActiveProjectPath.second = strpath;

			std::ifstream IProjectList("assets/Projects.json");
			json j;
			IProjectList >> j;
			IProjectList.close();
			std::ofstream OProjectList("assets/Projects.json");
			j[p_ActiveProjectPath.first] = {
				filepath,
				std::time(nullptr)
			};
			OProjectList << j;
			OProjectList.close();
		}
	}

	std::string EditorApplication::GetProjectFolder()
	{
		return Utils::FileDialogs::GetProjectFolder();
	}

	void EditorApplication::OnEvent(Event& e)
	{
		Application::OnEvent(e);
	}


	bool hasCprojFile(const std::string& folderPath) 
	{
		if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) 
		{
			std::cerr << "Invalid directory: " << folderPath << std::endl;
			return false;
		}

		for (const auto& entry : fs::directory_iterator(folderPath)) 
		{
			if (entry.is_regular_file() && entry.path().extension() == ".cproj") 
			{
				return true;
			}
		}
		return false;
	}



	void EditorApplication::Run()
	{
		std::map<int,std::pair<string,string>, std::greater<int>> ProjectList;
		ReadProjectsList(ProjectList);

		RenderCommand::SetViewport(50, 50,400,400);
		static char ProjectPath[256] = ""; // Make sure it's large enough to hold the path
		static char ProjectName[256] = ""; // Make sure it's large enough to hold the path

		bool ShowNewProjectWindow = false;
		while (p_Running)
		{
			p_ImGUILayer->Begin();
			p_Window->Begin();	

			CreateOpenWindow();

			ImGui::Begin("Projects");
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.01,0.5)); // Increase padding

			for (auto& s : ProjectList)
			{
				if (ImGui::Button((s.second.first + "\n" + s.second.second).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 50)))
				{
					p_ActiveProjectPath.first = s.second.first;
					p_ActiveProjectPath.second = s.second.second;
					break;
				}
			}
			ImGui::PopStyleVar();
			ImGui::End();

			ImGui::Begin("Project Options");
			if (ImGui::Button("New Project", ImVec2(ImGui::GetContentRegionAvail().x, 50)))
			{
				ShowNewProjectWindow = true;
				//NewProject();
			}
			if (ImGui::Button("Open Project",ImVec2(ImGui::GetContentRegionAvail().x, 50)))
			{
				OpenProject();
			}
			ImGui::End();			
			
			if (ShowNewProjectWindow)
			{
				ImGui::Begin("Browse");
				ImGui::InputText("Project Name", ProjectName, 50);
				ImGui::InputText("Project Path", ProjectPath, 50);
				ImGui::SameLine();
				if (ImGui::Button("Browse..."))
				{
					std::string filepath = GetProjectFolder();
					strncpy(ProjectPath, filepath.c_str(), sizeof(ProjectPath) - 1); // Copy safely
					ProjectPath[sizeof(ProjectPath) - 1] = '\0'; // Ensure null termination
				}

				if (ImGui::Button("Create", ImVec2(ImGui::GetContentRegionAvail().x / 2, 30)))
				{
					string strProjectName(ProjectName);
					string strProjectPath(ProjectPath);
					bool exits = false;
					if (!strProjectName.empty() && !strProjectPath.empty())
					{
						if (strProjectName.find(" ") != std::string::npos)
						{
							ShowToast("Project Name cannot have spaces");
						}
						else
						{
							for (auto& i : ProjectList)
							{
								if (i.second.second == strProjectPath)
								{
									exits = true;
								}
							}

							if (exits)
							{
								ShowToast("Project Already Exists");
							}
							else
							{
								if (hasCprojFile(strProjectPath))
								{
									ShowToast("Folder already contains a Cresta project");
								}
								else
								{
									json j;
									std::ifstream inFile("assets/Projects.json");
									if (inFile.is_open() && inFile.peek() != std::ifstream::traits_type::eof()) 
									{
										inFile >> j;
									}
									inFile.close();

									j[strProjectName] = {
											strProjectPath,
											std::time(nullptr)
										};

									std::ofstream outFile("assets/Projects.json");
									if (outFile.is_open()) 
									{
										outFile << std::setw(4) << j << std::endl; 
									}
									outFile.close();

									std::ofstream Create(strProjectPath + "\\" + strProjectName + ".cproj");
									Create.close();

									p_ActiveProjectPath.first = strProjectName;
									p_ActiveProjectPath.second = strProjectPath;
									ShowNewProjectWindow = false;
								}
							}
						}
					}
					else
					{
						ShowToast("Project Name or Project Path cannot be Empty");
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(ImGui::GetContentRegionAvail().x, 30)))
				{
					ShowNewProjectWindow = false;
				}

				ImGui::End();
			}
			RenderToasts();
			p_ImGUILayer->End();
			p_Window->End();

			if (!p_ActiveProjectPath.first.empty() && !p_ActiveProjectPath.second.empty())
			{
				Application::Run();
				break;
			}
		}
	}
}
