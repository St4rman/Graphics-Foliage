#include "gui.h"


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
	windSpeed = 0.0f;
}

gui::~gui() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

}

void gui::BufferGuiData() {

	ImGui_ImplOpenGL3_NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	POINT p{};
	GetCursorPos(&p);
	io.MousePos = ImVec2(p.x, p.y);

	ImGui::NewFrame();

	ImGui::Begin("Grass Rendering Tool.", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		int show = 1;
		ImGui::Text(u8"Hello, world! ");
		ImGui::SliderFloat("Wind Speed", &windSpeed, 0.0f, 200.0f);
		ImGui::Text("Press Tab to cycle Wind Direction");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImVec2 v = ImGui::GetWindowSize();  // v = {32, 48} ,   is tool small

	ImGui::Text("%f %f", v.x, v.y);
	ImGui::End();

}

//treated as update until further things requiroed
void gui::RenderGui() {

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_PLUS)) {
		windSpeed += 10.0f;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_MINUS)) {
		windSpeed -= 10.0f;
	}

}