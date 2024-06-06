#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	
	if (!initShaders())  return;
	if (!initTextures()) return;
	if (!initMeshes())   return;
	if (!initSceneNodes()) return;
	if (!initComputeShaders()) return;
	
	Vector3 heightmapSize = heightMap->GetHeightmapSize();

	camera = new Camera(-45.0f, 0.0f, heightmapSize * Vector3(0.5f, 5.0f, 0.5f));
	light = new Light(heightmapSize * Vector3(0.5f, 5.0, 0.5f), Vector4(1, 1, 1, 1), heightmapSize.x);

	localOrigin = heightmapSize * Vector3(0.5f, 5.0f, 0.5f);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	timer = parent.GetTimer();
	init = true;
}

Renderer::~Renderer(void) {

	delete camera;
	delete heightMap;
	delete root;
	delete quad;

	//ALL SHADERS!!!
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete sceneShader;

	delete light;

	//DELETE ALL TEXTURES!!!
	glDeleteTextures(1, &waterTex);
	glDeleteTextures(1, &earthTex);
	glDeleteTextures(1, &earthBump);
	glDeleteTextures(1, &debugTex);
}
void Renderer::ClearNodeLists() {
	transperentNodeList.clear();
	nodeList.clear();
}

bool Renderer::initTextures() {

	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	earthTex = SOIL_load_OGL_texture(TEXTUREDIR"BarrenReds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	earthBump = SOIL_load_OGL_texture(TEXTUREDIR"BarrenRedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	debugTex = SOIL_load_OGL_texture(TEXTUREDIR"test.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassTex = SOIL_load_OGL_texture(TEXTUREDIR"grassbush.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"left.png", TEXTUREDIR"right.png",
		TEXTUREDIR"up.png", TEXTUREDIR"down.png",
		TEXTUREDIR"front.png", TEXTUREDIR"back.png",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!earthTex || !earthBump || !cubeMap || !grassTex || !debugTex) {
		return false;
	}

	SetTextureRepeating(earthTex, true);
	SetTextureRepeating(earthBump, true);
	SetTextureRepeating(waterTex, true);
	return true;

}

bool Renderer::initShaders() {

	reflectShader = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
	skyboxShader  = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	lightShader   = new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");
	sceneShader   = new Shader("sceneVertex.glsl", "sceneFragment.glsl");
	gpuShader     = new Shader("gpuSceneVertex.glsl", "gpuSceneFragment.glsl");
	
	if (!reflectShader->LoadSuccess() || !skyboxShader->LoadSuccess() ||
		!lightShader->LoadSuccess()   || !sceneShader->LoadSuccess()  ||
		!gpuShader->LoadSuccess() ) {
		return false;
	}
	return true;
}

bool Renderer::initComputeShaders() {

	compShader = new ComputeShader("positionCompute.glsl");

	if (!compShader->LoadSuccess()) return false;

	glCreateBuffers(1, &ssboID);
	glNamedBufferStorage(ssboID, 3 * 2500* sizeof(GLfloat) * 4 * sizeof(GLfloat), 0, GL_DYNAMIC_STORAGE_BIT);

	return true;
}

bool Renderer::initMeshes(){

	quad = Mesh::GenerateQuad();
	triangle = Mesh::GenerateTriangle();
	heightMap = new HeightMap(TEXTUREDIR"white.jpg", { 5.0f, 1.0f, 5.0f });
	std::cout << heightMap->GetHeightmapSize();
	return heightMap->loadSuccess();

}

bool Renderer::initSceneNodes() {
	root = new SceneNode();
	root->SetTransform(Matrix4::Translation(localOrigin));

	for (int i = 0; i < 5; ++i) {
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
		s->SetTransform(Matrix4::Translation(
			Vector3(localOrigin.x + 500* i, localOrigin.y + 500, localOrigin.z)));
		s->SetModelScale({ 100.0f, 100.0f, 100.0f });
		s->SetBoundingRadius(100.0f);
		s->SetMesh(quad);
		s->SetTexture(earthTex);
		root->AddChild(s);
	}
	return true;
}

void Renderer::UpdateScene(float dt) {

	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);

	root->Update(dt);
}

void Renderer::BuildNodeLists(SceneNode* from) {
	if (frameFrustum.InsideFrustum(*from)) {

		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transperentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}	
}

void Renderer::SortNodeLists() {
	std::sort(transperentNodeList.rbegin(),
		transperentNodeList.rend(),
		SceneNode::CompareByCameraDistance);

	std::sort(nodeList.begin(),
		nodeList.end(),
		SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes() {
	for (const auto& i : nodeList) { DrawNode(i); }
	for (const auto& i : transperentNodeList) { DrawNode(i); }
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());

		glUniformMatrix4fv(
			glGetUniformLocation(sceneShader->GetProgram(), "modelMatrix"), 1, false, model.values);

		glUniform4fv(
			glGetUniformLocation(sceneShader->GetProgram(), "nodeColor"), 1, (float*)&n->GetColour());

		auto texture = n->GetTexture();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);

		n->Draw(*this);
	}
}

void Renderer::DrawSceneNodeItems() {

	BuildNodeLists(root);
	SortNodeLists();
	BindShader(sceneShader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "useTexture"), 1);
	DrawNodes();
	ClearNodeLists();

}

void Renderer::RenderScene() {

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawHeightMap();
	//DrawSceneNodeItems();
	DrawGrass();
	
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	UpdateShaderMatrices();

	quad->Draw();
	glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightMap() {

	BindShader(lightShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(lightShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex);

	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthBump);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->Draw();

}

void Renderer::DrawGrass() {

	int scale = 50;
	//compute shader bullshit
	Vector3 hSize = heightMap->GetHeightmapSize();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboID);
	
	compShader->Bind();
	compShader->Dispatch((unsigned int)scale, (unsigned int)scale, 1);
	glUniform3fv(glGetUniformLocation(compShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform3fv(glGetUniformLocation(compShader->GetProgram(), "mapSize"), 1, (float*)&hSize);
	glUniform1f(glGetUniformLocation(compShader->GetProgram(), "t"), (float)timer->GetTotalTimeSeconds());
	glUniform1i(glGetUniformLocation(compShader->GetProgram(), "scale"), (int)scale);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	BindShader(gpuShader);

	modelMatrix = Matrix4::Translation({ 0.0f, 225.0f + 80.0f, 0.0f }) * Matrix4::Scale({ 50.0f, 50.0f, 50.0f }) * Matrix4::Rotation(180, {0,0,1});
	textureMatrix.ToIdentity();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTex);
	glUniform1i(glGetUniformLocation(gpuShader->GetProgram(), "diffuseTex"), 0);
	glUniform1f(glGetUniformLocation(gpuShader->GetProgram(), "t"), timer->GetTotalTimeSeconds());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	UpdateShaderMatrices();	
	Mesh* qaub = Mesh::LoadFromMeshFile("grassequi.msh");
	qaub->DrawInstanced(scale*scale);
}