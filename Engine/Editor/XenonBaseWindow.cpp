#include "XenonBaseWindow.h"
#include "Library/imgui/imgui.h"

#include <stdio.h>
namespace XenonEngine {

int XenonBaseWindow::indexCount = 0;

    XenonBaseWindow::XenonBaseWindow(){
        m_index = indexCount++;
        m_open = false;
        SetParentWindow(nullptr);
    }
    XenonBaseWindow::XenonBaseWindow(XenonBaseWindow* parent)
    {
        m_index = indexCount++;
        m_open = false;
        SetParentWindow(parent);
    }

    XenonBaseWindow::~XenonBaseWindow()
    {
        m_open = false;
        SetParentWindow(nullptr);
    }

    void XenonBaseWindow::Update(const void* data)
    {
        if (!m_open)
        {
            return;
        }

        const char * windowName = GetWindiwName();



        //Only for popup
        if (GetWindowType() == Window_Type::Popup)
        {
            ImGui::OpenPopup(windowName);
            if (ImGui::BeginPopupModal(windowName, NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                UpdateMainWindow();
                ImGui::EndPopup();
            }
            return;
        }

        //For regular windows
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_MenuBar;

        switch (GetWindowType())
        {
		case Window_Type::SingleWindow:
        case Window_Type::MultiWindow:
        {
            // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

            break;
        }

        case Window_Type::SubWindow:
        {
            ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

            break;
        }

        default:
            break;
        }

        if (GetWindowType() == Window_Type::MultiWindow)
        {
            char name[128];
            int index = GetWindowIndex();
            sprintf_s(name, "%s_%d", windowName, index);
            // Main body of the Demo window starts here.
            if (!ImGui::Begin(name, &m_open, window_flags))
            {
                // Early out if the window is collapsed, as an optimization.
                ImGui::End();
                return;
            }
        }
        else
        {
            // Main body of the Demo window starts here.
            if (!ImGui::Begin(windowName, &m_open, window_flags))
            {
                // Early out if the window is collapsed, as an optimization.
                ImGui::End();
                return;
            }
        }


        // Most "big" widgets share a common width settings by default.
        //ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // Use 2/3 of the space for widgets and 1/3 for labels (default)
        ImGui::PushItemWidth(ImGui::GetFontSize() * -12);           // Use fixed width for labels (by passing a negative value), the rest goes to widgets. We choose a width proportional to our font size.

        UpdateMainWindow(data);

        //// Menu Bar
        //if (ImGui::BeginMenuBar())
        //{
        //    if (ImGui::BeginMenu("Menu"))
        //    {
        //        ImGui::EndMenu();
        //    }

        //    ImGui::EndMenuBar();
        //}


        ImGui::End();
    }

    void XenonBaseWindow::UpdateMainWindow(const void* data)
    {
        data;
        ImGui::Text("Placeholder");
    }

    //void XenonBaseWindow::UpdateMenu()
    //{
    //    //   ImGui::MenuItem("(dummy menu)", NULL, false, false);
    //    if (ImGui::MenuItem("New")) {

    //    }

    //    if (ImGui::MenuItem("Open", "Ctrl+O")) {

    //    }

    //    if (ImGui::MenuItem("Save", "Ctrl+S")) {

    //    }
    //}
    const char* XenonBaseWindow::GetWindiwName() const {
        return "WindowTemplate";
    }
    XenonBaseWindow::Window_Type XenonBaseWindow::GetWindowType() const
    {
        return Window_Type::MultiWindow;
    }

    void XenonBaseWindow::SetParentWindow(XenonBaseWindow * parent)
    {
        m_parent = parent;
    }


    bool*const XenonBaseWindow::GetHandle()
    {
        return &m_open;
    }

    void XenonBaseWindow::OpenWindow()
    {
        m_open = true;
    }

    void XenonBaseWindow::CloseWindow() {
        switch (GetWindowType())
        {

        case Window_Type::Popup:
        {
            ImGui::CloseCurrentPopup();
            break;
        }
        default:
            break;
        }

        m_open = false;
    }
    bool XenonBaseWindow::IsWindowOpen() const
    {
        return  m_open;
    }


}
