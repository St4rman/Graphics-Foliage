#include "gui.h"
#include <iostream>

gui::gui(int w, int h) {
	init = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	io.DisplaySize = ImVec2(w, h);

	ImGui::StyleColorsDark();

	ImGui_ImplOpenGL3_Init("#version 430");
	std::cout << "inited";
	init = true;
	windSpeed = 100.0f;
}

gui::~gui() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

}

void gui::BufferGuiData() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Grass Rendering Tool.");
	{
		int show = 1;
		ImGui::Text(u8"Hello, world! ");
		ImGui::SliderFloat("float", &windSpeed, 0.0f, 1.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImVec2 v = ImGui::GetWindowSize();  // v = {32, 48} ,   is tool small
	ImGui::Text("%f %f", v.x, v.y);
	ImGui::End();
}

void gui::RenderGui() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}