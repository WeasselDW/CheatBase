#include <iostream>
#include "Memory.cpp"
#include "Graphics.cpp"
#include "imgui/imgui.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam)) {
        return 0L;
    }

    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0L;
    }

    return DefWindowProc(window, message, wParam, lParam);
}


int main()
{
    Memory mem;
    Graphics graphics;
    graphics.InitializeWindow(window_procedure);
    if (mem.terminateProcess) {
        return(101);
    }
    
    // mainloop (Window)
    bool running = true;
    while (running) {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                running = false;
            }

        }
        if (!running) {
            break;
        }

        //Put code here |
        //             \|/
        graphics.RenderFrame();

    }
    // unload memory class / unhook
    delete &mem;
    // unload IMGUI
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext;


    return(0);

}