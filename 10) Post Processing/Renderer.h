#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Camera.h"

class Renderer : public OGLRenderer{
public: 
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene() override;
	void UpdateScene(float dt) override;

protected: 
	void PresentScene();
	void DrawPostProcess();
	void DrawScene();
	void DrawQuad();

	Shader* sceneShader;
	Shader* processShader;

	Camera* camera;

	Mesh* quad;
	Mesh* quad1;
	HeightMap* heightMap;
	GLuint heightTexture;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;
};

