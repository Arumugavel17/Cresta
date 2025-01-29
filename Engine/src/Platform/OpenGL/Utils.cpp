#include "Utils.hpp"
#include <shobjidl.h> // For SHBrowseForFolder

namespace Cresta
{
	namespace Utils
	{
		std::string FileDialogs::SelectFolder()
        {
            std::string selectedPath = "";
            // Initialize COM for using IFileDialog
            HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
            if (FAILED(hr)) {
                std::cerr << "Failed to initialize COM: " << std::hex << hr << std::endl;
                return "";
            }

            IFileSaveDialog* pFileSave = nullptr;

            // Create the File Save Dialog
            hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, (void**)&pFileSave);
            if (FAILED(hr)) {
                std::cerr << "Failed to create file dialog: " << std::hex << hr << std::endl;
                CoUninitialize();
                return "";
            }

            // Set file types for filtering
            COMDLG_FILTERSPEC fileTypes[] = {
                { L"Cresta Project Files", L"*.cproj" },
                { L"All Files", L"*.*" }
            };
            pFileSave->SetFileTypes(2, fileTypes);
            pFileSave->SetDefaultExtension(L"cproj");
            pFileSave->SetFileName(L"project.cproj");

            // Show the dialog
            hr = pFileSave->Show(nullptr);
            if (SUCCEEDED(hr)) {
                // Get the result
                IShellItem* pItem = nullptr;
                hr = pFileSave->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    // Get the file path
                    PWSTR pszFilePath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                    if (SUCCEEDED(hr)) {
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
            else {
                std::cerr << "Dialog canceled or failed." << std::endl;
            }

            // Release resources and uninitialize COM
            pFileSave->Release();
            CoUninitialize();

            return selectedPath;
        }

		std::string FileDialogs::OpenFile(const char* filter)
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

		std::string FileDialogs::SaveFile(const char* filter)
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
