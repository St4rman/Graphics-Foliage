#pragma once
#include "Renderer.h"
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Shader.h"
#include "../nclgl/Camera.h"


class Camera;
class Shader;
class HeightMap;


class Renderer : public OGLRenderer {
public: 
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene() override;
	void UpdateScene(float dt) override;

protected: 
	void DrawHeightMap();
	void DrawWater();
	void DrawSkybox();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	HeightMap* heightMap;
	Mesh* quad;

	Light* light;
	Camera* camera;

	GLuint cubeMap =0;
	GLuint waterTex = 0;
	GLuint earthTex = 0;
	GLuint earthBump = 0;

	float waterRotate = 0.0f;
	float waterCycle = 0.0f;
};

