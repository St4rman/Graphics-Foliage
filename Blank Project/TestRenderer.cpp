#include "TestRenderer.h"

TestRenderer::TestRenderer(Window &parent) : OGLRenderer(parent)	{
	triangle = Mesh::GenerateTriangle();
	testQuad = Mesh::GenerateQuad();

	basicShader = new Shader("basicVertex.glsl","basicFragment.glsl");
	basicTexture = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	compShader = new ComputeShader("basicCompute.glsl");

	if(!basicShader->LoadSuccess()) {
		return;
	}
	if (!basicTexture) {
		return;
	}
	if(!compShader->LoadSuccess()) {
		return;
	}

	glGenTextures(1, &ctexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ctexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, ctexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	

	uboID = glGetUniformBlockIndex(basicShader->GetProgram(), "Matrices");
	glUniformBlockBinding(basicShader->GetProgram(), uboID, 0);

	unsigned int uboMat;
	glGenBuffers(1, &uboMat);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMat);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMat, 0, 2 * sizeof(GLfloat));
	
	float temp = 1000.0f;
	glBindBuffer(GL_UNIFORM_BUFFER, uboMat);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat), &temp);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	float temp2 = 1.0f;
	glBindBuffer(GL_UNIFORM_BUFFER, uboMat);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat), sizeof(GLfloat), &temp2);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Vector4 temp3 =Vector4(0.0f,0.0f,0.0f,0.0f);
	glCreateBuffers(1, &shaderdBuffer);
	glNamedBufferStorage(shaderdBuffer, 4* sizeof(GLfloat), &temp3, GL_DYNAMIC_STORAGE_BIT);

	
	timer = parent.GetTimer();
	init = true;
}
TestRenderer::~TestRenderer(void)	{
	delete triangle;
	delete testQuad;
	delete basicShader;
	delete compShader;
	delete timer;
	glDeleteTextures(1, &ctexture);
}

void TestRenderer::UpdateScene(float dt) {

}

void TestRenderer::RenderScene()	{

	glClearColor(0.2f,0.2f,0.2f,1.0f);	

	glClear(GL_COLOR_BUFFER_BIT);	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, shaderdBuffer);
	
	compShader->Bind();
	glUniform1f(glGetUniformLocation(compShader->GetProgram(), "t"), (float)timer->GetTotalTimeSeconds());
	compShader->Dispatch((unsigned int)TEXTURE_SIZE/10, (unsigned int)TEXTURE_SIZE/10, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BindShader(basicShader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(basicShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ctexture);
	testQuad->Draw();
}

