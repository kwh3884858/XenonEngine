#pragma once
#include "imgui.h"
#include "Algorithms/String.h"
namespace ImGui
{
    IMGUI_API bool  InputText(const char* label, Algorithm::String& str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);

}