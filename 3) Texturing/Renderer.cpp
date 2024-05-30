#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	//as usual, we generate trangle but this time there are texture coords in there too 
	//poggers
	triangle = Mesh::GenerateTriangle();
	camera = new Camera;

	//lo2d thingading in the texture from the directory poggers
	texture = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	texture2 = SOIL_load_OGL_texture(TEXTUREDIR"mawio.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	blend = 0.5f;

	if (!texture && !texture2) {
		return;
	}

	//Grab shader from the files as given lol lmao 
	shader = new Shader("TexturedVertex.glsl", "TexturedFragment.glsl");

	if (!shader->LoadSuccess()) {
		return;
	}

	filtering = true;
	repeating = false;
	init = true;
}


Renderer::~Renderer(void) {
	delete triangle;
	delete shader;
	glDeleteTextures(1, &texture);
}

//ED NOTES:
//So, since we cant just rotate the texture randomly, we pop it out to the origin and then
//we rote it around origin and put it back by push 

void Renderer::UpdateTextureMatrix(float value) {

	Matrix4 push = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4 pop = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation(value, Vector3(0, 0, 1));

	textureMatrix = pop * rotation * push;

}

void Renderer::RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BindShader(shader);
	UpdateShaderMatrices(); //literally updates the shader matrices form the current binded shader

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "otherTex"), 1);

	glActiveTexture(GL_TEXTURE0); //makes the arguement texture active
	glBindTexture(GL_TEXTURE_2D, texture); //tells OGL that texture is of type texture 2D

	glActiveTexture(GL_TEXTURE1); //makes the arguement texture active
	glBindTexture(GL_TEXTURE_2D, texture2);


	glUniform1f(glGetUniformLocation(shader->GetProgram(), "blendnum"), blend);


	triangle->Draw();
}

void Renderer::ToggleRepeating() {
	repeating = !repeating;
	SetTextureRepeating(texture, repeating);
}

//switches between bilinear filtering and nearest neighbour filtering
void Renderer::ToggleFiltering() {
	filtering = !filtering;
	glBindTexture(GL_TEXTURE_2D, texture);

	//these are doin LOD i think 
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST);*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, SOIL_FLAG_MIPMAPS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, SOIL_FLAG_MIPMAPS);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Renderer::SetBlend(float change) {
	blend += change;
	return;
}


void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	/*modelMatrix = position;*/
	//projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}