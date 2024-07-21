#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "../NCLGL/window.h"

class gui
{
public:
	gui(int w, int h);
	~gui();

	void BufferGuiData();
	void RenderGui();
	bool hasGuiInit()		const { return init; }
	float returnWSpeed()	const { return windSpeed; }

protected:
	bool init; 
	float windSpeed;
	Vector2 windDir;
};

