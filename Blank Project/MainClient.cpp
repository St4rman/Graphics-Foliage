
#include "../NCLGL/window.h"
#include "Renderer.h"
#include "TestRenderer.h"
#include "gui.h"

int main()	{
	Window w("Grass time!", 1280, 720, false);
	

	if(!w.HasInitialised()) {
		return -1;
	}
	

	Renderer Renderer(w);	

	if(!Renderer.HasInitialised()) {
		return -1;
	}
	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);


	gui* GUI = new gui(1280, 720);
	if (!GUI->hasGuiInit()) {
		return -1;
	}

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		Renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		Renderer.RenderScene();

		GUI->BufferGuiData();
		GUI->RenderGui();

		Renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
	}

	delete GUI;
	return 0;
}