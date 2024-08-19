#include "../NCLGL/window.h"
#include "Renderer.h"
#include "TestRenderer.h"
#include "gui.h"

int main()	{
	Window w("Grass time!", 1280, 720, false);
	bool showMouse = false;
	w.LockMouseToWindow(true);
	w.ShowOSPointer(showMouse);
	

	if(!w.HasInitialised()) {
		return -1;
	}
	

	Renderer Renderer(w);	

	if(!Renderer.HasInitialised()) {
		return -1;
	}
	

	gui* GUI = new gui(1280, 720);
	if (!GUI->hasGuiInit()) {
		return -1;
	}

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){

		Renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		Renderer.RenderScene();

		GUI->BufferGuiData();
		GUI->RenderGui();
		GUI->UpdateCamPos(Renderer.getCamPos());

		Renderer.SwapBuffers();

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F2)) {
			Renderer.updateWindSpeed(GUI->returnWSpeed() / 1000.0f);
			Renderer.updateWindDir(GUI->returnWDir());
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F3)) {
			showMouse != showMouse;
			w.ShowOSPointer(true);
		}
	}

	delete GUI;
	return 0;
}