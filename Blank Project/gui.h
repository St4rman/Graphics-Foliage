#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"

class gui
{
public:
	gui(int w, int h);
	~gui();

	void BufferGuiData();
	void RenderGui();
	bool hasGuiInit() { return init; }

protected:
	bool init; 
};

