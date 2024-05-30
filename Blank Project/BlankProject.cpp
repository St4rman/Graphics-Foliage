#include "../NCLGL/window.h"
#include "TestRenderer.h"

int main()	{
	Window w("Make your own project!", 1280, 720, false);

	if(!w.HasInitialised()) {
		return -1;
	}
	
	TestRenderer tRenderer(w);
	if(!tRenderer.HasInitialised()) {
		return -1;
	}

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		tRenderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		tRenderer.RenderScene();
		tRenderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
	}
	return 0;
}