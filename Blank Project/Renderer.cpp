#include "Renderer.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	basicShader		= new Shader("basicVertex.glsl", "basicFragment.glsl");
	texturedShader	= new Shader("texturedShader.glsl", "texturedFragment.glsl");
	skyboxShader	= new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");

	if (!texturedShader->LoadSuccess()) {
		return;
	}

	if (!InitMeshes())	 return; 
	if (!InitTextures()) return;


	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	EnableDepthTest(true);
	EnableCullFace(true);
	glCullFace(GL_BACK);
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

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"left.png", TEXTUREDIR"right.png",
		TEXTUREDIR"up.png", TEXTUREDIR"down.png",
		TEXTUREDIR"front.png", TEXTUREDIR"back.png",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0
	);

	groundTex = SOIL_load_OGL_texture(TEXTUREDIR"BarrenReds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	if (!cubeMap || !groundTex) return false;

	SetTextureRepeating(groundTex, true);

	return true;
}


Renderer::~Renderer(void) {
	delete triangle;
	delete quad;
	delete heightMap;

	delete basicShader;
	delete texturedShader;
	delete skyboxShader;
}

void Renderer::RenderScene() {

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	BindShader(basicShader);
	triangle->Draw();

}