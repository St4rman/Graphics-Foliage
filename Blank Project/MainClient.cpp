
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include "../NCLGL/window.h"
#include "Renderer.h"
#include "TestRenderer.h"

int main()	{
	Window w("Grass time!", 1280, 720, false);

	if(!w.HasInitialised()) {
		return -1;
	}

	bool debug = false;
	Renderer Renderer(w);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(1280, 720);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 430");


	if(!Renderer.HasInitialised()) {
		return -1;
	}
	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		Renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		Renderer.RenderScene();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		//ImGui::Begin("this is a temp window", 0, 100);
		ImGui::Text("Hello, world!");
		//ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	return 0;
}