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

	windSpeed = 20.0f;
	windDir = { 0,1 };
}

gui::~gui() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

}

void gui::BufferGuiData() {

	ImGui_ImplOpenGL3_NewFrame();

	/*ImGuiIO& io = ImGui::GetIO();
	POINT p{};
	GetCursorPos(&p);
	io.MousePos = ImVec2(p.x, p.y);*/

	ImGui::NewFrame();
	/*ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(1.0f, 1.0f));*/

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Grass Rendering Tool.", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		int show = 1;
		ImGui::Text(u8"Press + and - to adjust the wind speed! %i", (int)windSpeed);
		ImGui::SliderFloat("Wind Speed", &windSpeed, 0.0f, 200.0f);
		ImGui::Text("Press Tab to cycle Wind Direction");
		ImGui::SameLine();
		ImGui::ArrowButton("##Right", ImGuiDir(index));
		/*ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Camera Pos: %f x, %f y, %f z",cameraPos.x, cameraPos.y, cameraPos.z);*/
	}
	
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
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_TAB)) {
		index += 1;
		if (index == WIND_DIR.size()) { index = 0; }
		windDir = WIND_DIR[index];
	}

	windSpeed = std::max(0.0f, std::min(windSpeed, 200.0f));

}