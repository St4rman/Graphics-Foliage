#include "Renderer.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	basicShader = new Shader("basicVertex.glsl", "basicFragment.glsl");

	if (!InitMeshes())	 return; 
	if (!InitTextures()) return;
	init = true;
}

bool Renderer::InitMeshes() {

	triangle  = Mesh::GenerateTriangle();
	quad	  = Mesh::GenerateQuad();
	heightMap = new HeightMap(TEXTUREDIR"noise.png");
	
	if (!heightMap->loadSuccess()) return false;
	return true;
}

bool Renderer::InitTextures() {
	return true;
}


Renderer::~Renderer(void) {
	delete triangle;
	glDeleteShader(basicShader->GetProgram());
}

void Renderer::RenderScene() {

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	BindShader(basicShader);
	triangle->Draw();

}