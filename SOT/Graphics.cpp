#pragma once
#include "imgui/imgui.h"
#include <windows.h>
#include <dwmapi.h>
#include <d3d11.h>
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

class Graphics
{
public:
	bool MenuOpen = false;
	IDXGISwapChain* swap_chain{nullptr};
	ID3D11RenderTargetView* render_target_view{ nullptr };
	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* device_context{ nullptr };
	HWND window;
	WNDCLASSEXW wc{};
	int screenWidth;
	int screenHeight;

	Graphics() {
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	~Graphics() {
		if (swap_chain) {
			swap_chain->Release();
		}
		if (render_target_view) {
			render_target_view->Release();
		}
		if (device) {
			device->Release();
		}
		if (device_context) {
			device_context->Release();
		}
		DestroyWindow(window);
		UnregisterClassW(wc.lpszClassName, wc.hInstance);
	}
	void InitializeWindow(WNDPROC winProc) {


		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = winProc;
		wc.lpszClassName = L"External Overlay Class";

		RegisterClassExW(&wc);
		
		window = CreateWindowExW(
			WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
			wc.lpszClassName,
			L"CHANGECHEATNAME",
			WS_POPUP,
			0, 0, screenWidth, screenHeight,
			nullptr,
			nullptr,
			wc.hInstance,
			nullptr
		);
		SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

		{
			RECT client_area{};
			GetClientRect(window, &client_area);

			RECT window_area{};
			GetWindowRect(window, &window_area);

			POINT diff{};
			ClientToScreen(window, &diff);

			const MARGINS margins{
				window_area.left + (diff.x - window_area.left),
				window_area.top + (diff.y - window_area.top),
				client_area.right,
				client_area.bottom
			};

			DwmExtendFrameIntoClientArea(window, &margins);
		}

		DXGI_SWAP_CHAIN_DESC sd{};
		sd.BufferDesc.RefreshRate.Numerator = 60U;
		sd.BufferDesc.RefreshRate.Denominator = 1U;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1U;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2U;
		sd.OutputWindow = window;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		constexpr D3D_FEATURE_LEVEL levels[2]{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_0

		};


		D3D_FEATURE_LEVEL level{};

		D3D11CreateDeviceAndSwapChain(
			nullptr, 
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0U,
			levels,
			2U,
			D3D11_SDK_VERSION,
			&sd,
			&swap_chain,
			&device,
			&level,
			&device_context
		);
		ID3D11Texture2D* back_buffer{ nullptr };
		swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

		if (back_buffer) {
			device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
			back_buffer->Release();
		}
		else {
			return;
		}

		ShowWindow(window, 1);
		UpdateWindow(window);

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(device, device_context);


	}

	// Imgui Variables
	bool some_variable = false;
	float some_float = 0.f;




	void RenderFrame() {

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


	// draw code goes here |
	//  				  \|/

		// Menu code here
		if (MenuOpen) {
			ImGui::Begin("Test window", NULL, ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
			ImGui::Text("Hello, World");
			ImGui::Checkbox("Toggle", &some_variable);
			ImGui::Button("Press");
			ImGui::SliderFloat("float", &some_float, 0.0f, 1.0f);
			ImGui::End();
		}
		// ESP code here
		ImGui::GetBackgroundDrawList()->AddCircleFilled({ sXP(50.f) , sYP(50.f) }, 10.f, ImColor(1.f, 0.f, 0.f));
	// end of drawing
	 
	 
	
		//Rendering
		ImGui::Render();
		constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
		device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
		device_context->ClearRenderTargetView(render_target_view, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swap_chain->Present(1U, 0U);

	}

	void OpenGui() {
		if (MenuOpen)
		{
			SetWindowLong(window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED);
			SetForegroundWindow(window);
		}else{
			SetWindowLong(window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT);
		}
	}


private:
	//Screen X Percentage
	float sXP(float percentage) {
		return (float)screenWidth / 100.f * percentage;
	}
	//Screen Y Percentage
	float sYP(float percentage) {
		return (float)screenHeight / 100.f * percentage;
	}
};


