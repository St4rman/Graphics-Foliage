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
}

gui::~gui() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

}

void gui::BufferGuiData() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	////ImGui::Begin("this is a temp window", 0, 100);
	ImGui::Text("Hello, world!");
	////ImGui::End();
}

void gui::RenderGui() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}