#include <vector>
#include <string>
#include <chrono>
#include <imgui/imgui.h>

namespace Editor
{
    struct Toast 
    {
        std::string Message;
        std::chrono::steady_clock::time_point StartTime;
        float Duration; // In seconds

        Toast(const std::string& msg, float durationSec)
            : Message(msg), Duration(durationSec), StartTime(std::chrono::steady_clock::now()) {
        }
    };

    // Global toast list
    std::vector<Toast> toasts;

    // Function to add a toast
    void ShowToast(const std::string& message, float duration = 3.0f) {
        toasts.emplace_back(message, duration);
    }

    // Function to render toasts
    void RenderToasts() 
    {
        float yOffset = 600.0f; // Starting position from top-right

        for (size_t i = 0; i < toasts.size();) {
            auto elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - toasts[i].StartTime).count();

            if (elapsed >= toasts[i].Duration) {
                toasts.erase(toasts.begin() + i); // Remove expired toast
                continue;
            }

            float alpha = 1.0f - (elapsed / toasts[i].Duration); // Fade out effect

            ImGui::SetNextWindowBgAlpha(alpha); // Adjust transparency
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 700, yOffset), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300, 60)); // Bigger size

            ImGui::Begin(("##toast_" + std::to_string(i)).c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("%s", toasts[i].Message.c_str());
            ImGui::End();

            yOffset += 40.0f; // Offset next toast
            ++i;
        }
    }
}