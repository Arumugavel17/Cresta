#include "SceneHierarchyPanelUtils.hpp"


namespace Cresta
{
	namespace Utils
	{
		void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			// Define reusable styles
			ImVec4 resetButtonColor = { 0.8f, 0.1f, 0.15f, 1.0f };
			ImVec4 resetButtonHoverColor = { 0.9f, 0.2f, 0.2f, 1.0f };
			ImVec4 resetButtonActiveColor = { 0.8f, 0.1f, 0.15f, 1.0f };

			ImVec4 xButtonColor = { 0.2f, 0.7f, 0.2f, 1.0f };
			ImVec4 xButtonHoverColor = { 0.3f, 0.8f, 0.3f, 1.0f };
			ImVec4 xButtonActiveColor = { 0.2f, 0.7f, 0.2f, 1.0f };

			ImVec4 zButtonColor = { 0.1f, 0.25f, 0.8f, 1.0f };
			ImVec4 zButtonHoverColor = { 0.2f, 0.35f, 0.9f, 1.0f };
			ImVec4 zButtonActiveColor = { 0.1f, 0.25f, 0.8f, 1.0f };

			ImGui::PushID(label.c_str());
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);

			// Render label
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 8.0f, 4.0f });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 4.0f, lineHeight };

			float availableWidth = ImGui::GetContentRegionAvail().x;  // Get available width for the current window
			// Function to create a styled button with drag float
			auto createStyledButton = [&](std::string buttonLabel, ImVec4 color, ImVec4 hoverColor, ImVec4 activeColor, float* value) {
				ImGui::PushStyleColor(ImGuiCol_Button, color);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
				ImGui::PushFont(boldFont);

				if (ImGui::Button(buttonLabel.substr(2, -1).c_str(), buttonSize))
					*value = resetValue;

				ImGui::PopFont();
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				std::string dragFloatID = buttonLabel + "DrawFloat";

				float dragWidth = availableWidth * 0.2f;
				ImGui::PushItemWidth(dragWidth);  // Set width for DragFloat
				ImGui::DragFloat(dragFloatID.c_str(), value, 0.1f, 0.0f, 0.0f, "%.2f");
				ImGui::PopItemWidth(); // Restore the previous width
				};

			// Render X button and value
			createStyledButton("##X", resetButtonColor, resetButtonHoverColor, resetButtonActiveColor, &values.x);
			ImGui::SameLine();
			// Render Y button and value
			createStyledButton("##Y", xButtonColor, xButtonHoverColor, xButtonActiveColor, &values.y);
			ImGui::SameLine();
			// Render Z button and value
			createStyledButton("##Z", zButtonColor, zButtonHoverColor, zButtonActiveColor, &values.z);

			ImGui::PopStyleVar();
			ImGui::Columns(1);
			ImGui::PopID();
		}

	}
}