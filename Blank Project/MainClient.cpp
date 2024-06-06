#include "../NCLGL/window.h"
#include "Renderer.h"
#include "TestRenderer.h"

int main()	{
	Window w("Grass time!", 1280, 720, false);

	if(!w.HasInitialised()) {
		return -1;
	}

	bool debug = false;
	//TestRenderer tRenderer(w);
	Renderer Renderer(w);

	/*if(!tRenderer.HasInitialised()) {
		return -1;
	}*/
	if(!Renderer.HasInitialised()) {
		return -1;
	}
	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		Renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		Renderer.RenderScene();
		Renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
	}
	return 0;
}