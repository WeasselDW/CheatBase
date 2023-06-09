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
    Memory* mem = new Memory(L"cmd.exe");
    Graphics* graphics = new Graphics();

    graphics->InitializeWindow(window_procedure);
    
    // mainloop (Window)
    // init main loop
    bool running = true;
    while (running) {
        // check if quit button is clicked
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

        //Put main loop code here |
        //                       \|/
        graphics->RenderFrame();
        mem->MemoryLoop();
        if (GetAsyncKeyState(VK_HOME) & 1) {
            graphics->menu->MenuOpen = !graphics->menu->MenuOpen;
            graphics->OpenGui();
        }


        
    }
    //delete Class objects
    delete &mem;
    delete &graphics;
    // unload IMGUI
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext;


    return(0);

}