#pragma once
#include <iostream>
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/HeightMap.h"

class Renderer : public OGLRenderer{
public: 
	Renderer(Window& parent);
	virtual ~Renderer(void);

	bool InitShaders();
	bool InitTextures();
	bool InitMeshes();


	virtual void RenderScene();

protected: 
	Mesh*		triangle;
	Mesh*		quad;
	HeightMap*	heightMap;
	

	Shader*		basicShader;
	Shader*		skyboxShader;
	Shader*		texturedShader;
	
	GLuint		cubeMap       = 0;
	GLuint		groundTex     = 0;
};

