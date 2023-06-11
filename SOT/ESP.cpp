#pragma once
#include "./imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Menu.cpp"
#include <Windows.h>
#include <WinUser.h>
class ESP {

	int screenWidth;
	int screenHeight;
public:
	ImDrawList* BGDraw;
	Menu* menu = NULL;


	ESP(Menu* _menu) {
		menu = _menu;
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	void RenderESP(ImDrawList* DrawList) {
		BGDraw = DrawList;
		if (menu->Show_Crosshair) {
			Crosshair(menu->Crosshair_Length, menu->Crosshair_Color, menu->Crosshair_Thickness);
		}

	}



private:
	void Crosshair(float Crosshair_Length, ImColor Crosshair_Color, float Crosshair_Thickness) {
		float drX = screenWidth / 2.f;
		float drY = screenHeight / 2.f;

		BGDraw->AddLine({ drX - Crosshair_Length, drY }, { drX + Crosshair_Length, drY }, Crosshair_Color, Crosshair_Thickness);
		BGDraw->AddLine({ drX, drY - Crosshair_Length }, { drX, drY + Crosshair_Length }, Crosshair_Color, Crosshair_Thickness);

	}
};