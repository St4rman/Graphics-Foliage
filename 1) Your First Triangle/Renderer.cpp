#include "Renderer.h"
#include <iostream>

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	triangle = Mesh::GenerateTriangle();

	basicShader = new Shader("basicVertex.glsl", "basicFragment.glsl");


	if (!basicShader->LoadSuccess()) {
		return;
	}

	init = true;
}

Renderer::~Renderer(void) {
	delete triangle;
	delete basicShader;
}

void Renderer::RenderScene() {

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	BindShader(basicShader);
	triangle->Draw();
	//tri2->Draw();
}