#pragma once
#include "../NCLGL/OGLRenderer.h"
#include "../nclgl/ComputeShader.h"
#include "../nclgl/GameTimer.h"
const unsigned int TEXTURE_SIZE = 1000;

class TestRenderer : public OGLRenderer	{
public:
	TestRenderer(Window &parent);
	 ~TestRenderer(void);
	 void RenderScene()				override;
	 void UpdateScene(float msec)	override;
protected:
	Mesh*	triangle;
	Mesh*   testQuad;
	Shader* basicShader;
	GLuint	ctexture;
	GLuint	basicTexture;
	ComputeShader* compShader;
	GameTimer* timer;
	GLuint  uboID;

	GLuint shaderdBuffer;
};
