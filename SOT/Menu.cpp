#include "./imgui/imgui.h"
#include "imgui/imgui_internal.h"
#pragma once
class Menu {

	bool first_time = true;
	ImGuiID dockspace_id = NULL;
	ImGuiViewport* viewport = NULL;



public:

	bool MenuOpen = false; // if menu key is pressed


	bool ESP_Enabled = false;
	bool CHAMS_Enabled = false;

	//Crosshair
	bool Show_Crosshair = false;
	float Crosshair_Thickness = 1.f;
	float Crosshair_Length = 10.f;
	ImColor Crosshair_Color = { 255,0,0,255 };



	void RenderMenu() {

		if (MenuOpen) {

			viewport = ImGui::GetMainViewport();
			ImGui::Begin("CheatBase dockspace", NULL, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			dockspace_id = ImGui::GetID("CheatBase dockspace");
			if (first_time)
			{

				ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
				ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoWindowMenuButton);
				ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
			}
			if (ImGui::BeginTabBar("Cheat"))
			{
				ImGui::DockSpace(dockspace_id, { 800, 600 }, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton);

				if (ImGui::BeginTabItem("Visuals"))
				{
					RenderVisuals();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Aimbot"))
				{
					RenderAimbot();
					ImGui::EndTabItem();

				}
				if (ImGui::BeginTabItem("Misc"))
				{
					RenderMisc();
					ImGui::EndTabItem();

				}
				ImGui::EndTabBar();
			}

			first_time = false;
			ImGui::End();
		}
	}


private:
	//Tabbar items
	void RenderAimbot() {
		ImGui::Begin("Silent", NULL);
		ImGui::Checkbox("Enabled", &ESP_Enabled);
		ImGui::End();

		ImGui::Begin("Rage", NULL);
		ImGui::Checkbox("Enabled", &CHAMS_Enabled);
		ImGui::End();

		ImGui::DockBuilderDockWindow("Silent", dockspace_id);
		ImGui::DockBuilderDockWindow("Rage", dockspace_id);
	}
	void RenderVisuals() {
		ImGui::Begin("ESP", NULL);
		ImGui::Checkbox("Enabled", &ESP_Enabled);
		ImGui::End();

		ImGui::Begin("CHAMS", NULL);
		ImGui::Checkbox("Enabled", &CHAMS_Enabled);
		ImGui::End();


		ImGui::DockBuilderDockWindow("ESP", dockspace_id);
		ImGui::DockBuilderDockWindow("CHAMS", dockspace_id);
	}
	void RenderMisc() {
		ImGui::Begin("Crosshair", NULL);
		ImGui::Checkbox("Show Crosshair", &Show_Crosshair);
		if (Show_Crosshair) {
			if (ImGui::CollapsingHeader("Crosshair settings"))
			{
				ImGui::SliderFloat("Thickness", &Crosshair_Thickness, 0.1f, 5.f);
				ImGui::SliderFloat("Length", &Crosshair_Length, 1.f, 50.f);
				ImGui::ColorEdit4("Color", &Crosshair_Color.Value.x);

			}

		}
		ImGui::End();

		ImGui::Begin("Profiles", NULL);
		ImGui::Text("INOP");
		ImGui::Button("Save");
		ImGui::Button("Load");
		ImGui::End();

		ImGui::DockBuilderDockWindow("Crosshair", dockspace_id);
		ImGui::DockBuilderDockWindow("Profiles", dockspace_id);
	}



};
