#include "Utils.hpp"
#include "Utils.hpp"
#include <shobjidl.h> // For SHBrowseForFolder

namespace Cresta
{
	namespace Utils
	{
        std::string FileDialogs::OpenProject()
        {
            std::string selectedPath = ""; // Default file if none is selected

            // Initialize COM for using IFileDialog
            HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
            if (FAILED(hr))
            {
                std::cerr << "Failed to initialize COM: " << std::hex << hr << std::endl;
                return selectedPath;
            }

            IFileOpenDialog* pFileOpen = nullptr;

            // Create the File Save Dialog
            hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&pFileOpen);
            if (FAILED(hr))
            {
                std::cerr << "Failed to create file dialog: " << std::hex << hr << std::endl;
                CoUninitialize();
                return selectedPath;
            }

            // Set file types for filtering
            COMDLG_FILTERSPEC fileTypes[] = {
                { L"Cresta Project Files", L"*.cproj" },
                { L"All Files", L"*.*" }
            };
            pFileOpen->SetFileTypes(2, fileTypes);
            pFileOpen->SetDefaultExtension(L"cproj");
            pFileOpen->SetFileName(L"project.cproj");

            // Show the dialog
            hr = pFileOpen->Show(nullptr);
            if (SUCCEEDED(hr))
            {
                // Get the result
                IShellItem* pItem = nullptr;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    // Get the file path
                    PWSTR pszFilePath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                    if (SUCCEEDED(hr))
                    {
                        // Convert the wide string to a standard string
                        char filepath[MAX_PATH];
                        wcstombs(filepath, pszFilePath, MAX_PATH);

                        // Store the selected path
                        selectedPath = filepath;

                        // Free memory
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            else
            {
                std::cerr << "Dialog canceled or failed. Using default project file: " << selectedPath << std::endl;
                return "";
            }

            // Release resources and uninitialize COM
            pFileOpen->Release();
            CoUninitialize();
            return selectedPath;
        }

        std::string FileDialogs::GetProjectFolder()
        {
            std::string selectedPath = "";  // Will store the selected directory

            // Initialize COM for using IFileDialog
            HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
            if (FAILED(hr))
            {
                std::cerr << "Failed to initialize COM: " << std::hex << hr << std::endl;
                return "";
            }

            IFileOpenDialog* pFileDialog = nullptr;

            // Create the File Dialog for folder selection
            hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&pFileDialog);
            if (FAILED(hr))
            {
                std::cerr << "Failed to create file dialog: " << std::hex << hr << std::endl;
                CoUninitialize();
                return "";
            }

            // Set the dialog to pick folders instead of files
            DWORD dwOptions;
            pFileDialog->GetOptions(&dwOptions);
            pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

            // Show the dialog
            hr = pFileDialog->Show(nullptr);
            if (SUCCEEDED(hr))
            {
                // Get the selected folder
                IShellItem* pItem = nullptr;
                hr = pFileDialog->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFolderPath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
                    if (SUCCEEDED(hr))
                    {
                        // Convert the wide string to a standard string
                        char folderPath[MAX_PATH];
                        wcstombs(folderPath, pszFolderPath, MAX_PATH);

                        // Store the selected path
                        selectedPath = folderPath;

                        std::ofstream project(selectedPath + "\\pr");
                        

                        // Free memory
                        CoTaskMemFree(pszFolderPath);
                    }
                    pItem->Release();
                }
            }
            else
            {
                std::cerr << "Dialog canceled or failed." << std::endl;
            }

            // Release resources and uninitialize COM
            pFileDialog->Release();
            CoUninitialize();

            return selectedPath;
        }

		std::string FileDialogs::OpenScene(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetApplication().GetWindow()->GetWindowContext());
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;

			return std::string();

		}

		std::string FileDialogs::SaveScene(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetApplication().GetWindow()->GetWindowContext());
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

			// Sets the default extension by extracting it from the filter
			ofn.lpstrDefExt = strchr(filter, '\0') + 1;

			if (GetSaveFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;

			return std::string();
		}


	}
}
