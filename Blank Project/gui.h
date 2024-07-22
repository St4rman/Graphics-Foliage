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
	Vector2 returnWDir()	const { return windDir; }

protected:
	bool init; 
	int index = 0;
	float windSpeed;
	Vector2 windDir;
	std::vector<Vector2> WIND_DIR = { { -1,  0},  { 1,  0}, { 0,  1}, { 0, -1}, { -1, 1}, {1, 1}, {-1,  -1}, {1, -1} };
	std::vector<ImGuiDir> curDir = { ImGuiDir_Up, };
};

