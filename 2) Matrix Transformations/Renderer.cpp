#include "Renderer.h"
#include "../nclgl/Camera.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	camera = new Camera;

	tri = Mesh::GenerateTriangle();


	matrixShader = new Shader("viewVertex.glsl", "basicFragment.glsl");

	if (!matrixShader->LoadSuccess()) {
		return;
	}
	init = true;
	SwitchToOrthographic();
}

void Renderer::UpdateScene(float dt) {

	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
}

Renderer::~Renderer(void) {
	delete tri;
	delete matrixShader;
}





//this is setting an OGL variable to an actual prespective matrix which is returned by using the formula of said thing lol 
//Look up formula of projectino matrix thats just all thats used here
void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}


//this is also a formula thats been thought out? maybe im not sure
//ask if these formulas are like set always or if i have to deruive thgen 
//this has a minus one as a value because its sometimes used to render HUD and UI elements

void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

float i = 45.0f;
void Renderer::IncreaseFov() {
	i += 5.0f;
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, i);
}

void Renderer::DecreaseFov() {
	i -= 5.0f;
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, i);
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT);


	//this is a custom rich func that just sets the shader u pass in as the current state shader
	BindShader(matrixShader);


	//program, which is where the variable projmatrix is stored
	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "projMatrix"), 1, false, projMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "viewMatrix"), 1, false, viewMatrix.values);

	for (int i = 0; i < 3; i++) {
		Vector3 tempos = position;
		tempos.z += (i * 500.0f);
		tempos.x -= (i * 100.0f);
		tempos.y -= (i * 100.0f);


		modelMatrix = Matrix4::Translation(tempos) * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(scale, scale, scale));

		glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);

		tri->Draw();

	}
}