#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	SCALE = { 80, 80 };
	TOTALDISPATCH = SCALE.x * SCALE.y * 10 * 10;

	if (!initShaders())  return;
	if (!initTextures()) return;
	if (!initMeshes())   return;
	if (!initComputeShaders()) return;
	
	heightmapSize = heightMap->GetHeightmapSize();
	

	camera = new Camera(-45.0f, 0.0f, {0,0,0});
	light = new Light(heightmapSize* Vector3(0.5f, heightmapSize.x * 0.5f / heightmapSize.y, -0.5f), Vector4(1, 1, 1, 1), heightmapSize.x);

	if (!initSceneNodes()) return;

	localOrigin = heightmapSize * Vector3(0.5f, 5.0f, 0.5f);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	timer = parent.GetTimer();
	
	windSpeed = 0.02;
	windDir   = { 0 , 1 };
	windFwdSway = 90;
	windRightSway = 70;

	grassDimensions = { 3, 15, 3 };
	init = true;
	
}

Renderer::~Renderer(void) {

	delete heightMap;
	delete root;
	delete quad;
	delete triangle;
	delete grassMesh;
	delete light;
	delete camera;
	delete timer;

	//ALL SHADERS!!!
	delete lightShader;
	delete reflectShader;
	delete skyboxShader;
	delete sceneShader;
	delete gpuShader;
	delete compShader;

	//DELETE ALL TEXTURES!!!
	glDeleteTextures(1, &cubeMap);
	glDeleteTextures(1, &waterTex);
	glDeleteTextures(1, &earthTex);
	glDeleteTextures(1, &debugTex);
	glDeleteTextures(1, &earthBump);
	glDeleteTextures(1, &grassTex);
	glDeleteTextures(1, &compVnoise);
}
void Renderer::ClearNodeLists() {
	transperentNodeList.clear();
	nodeList.clear();
}

bool Renderer::initTextures() {

	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	earthTex = SOIL_load_OGL_texture(TEXTUREDIR"base_grass.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	earthBump = SOIL_load_OGL_texture(TEXTUREDIR"base_grassN.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	debugTex = SOIL_load_OGL_texture(TEXTUREDIR"test.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassTex = SOIL_load_OGL_texture(TEXTUREDIR"grassbush.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	windTex  = SOIL_load_OGL_texture(TEXTUREDIR"wind-bump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"left.png", TEXTUREDIR"right.png",
		TEXTUREDIR"up.png", TEXTUREDIR"down.png",
		TEXTUREDIR"front.png", TEXTUREDIR"back.png",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!earthTex || !earthBump || !cubeMap || !grassTex || !debugTex || !windTex) {
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
	glNamedBufferStorage(ssboID, 3 * TOTALDISPATCH * sizeof(GLfloat) * 4 * sizeof(GLfloat), 0, GL_DYNAMIC_STORAGE_BIT);


	if (!heightMap->loadSuccess()) return false;
	

	std::vector<float> temp = heightMap->GetVerticalOffset();
	std::cout << "vert length offset" << temp.size();
	std::reverse(temp.begin(), temp.end());
	glCreateBuffers(1, &heightBuffer);
	glNamedBufferStorage(heightBuffer, temp.size() * sizeof(GLfloat),reinterpret_cast<GLfloat*>(temp.data()), GL_DYNAMIC_STORAGE_BIT);



	glGenTextures(1, &compVnoise);
	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, compVnoise);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 200, 200, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, compVnoise, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);


	return true;
}

bool Renderer::initMeshes(){

	quad		= Mesh::GenerateQuad();
	triangle	= Mesh::GenerateTriangle();
	triangle->GenerateNormals();
	triangle->GenerateTangents();
	heightMap	= new HeightMap(TEXTUREDIR"noise3.png", { 10.0f, 10.0f, 10.0f });
	grassMesh	= Mesh::LoadFromMeshFile("GrassVert.msh");

	return heightMap->loadSuccess();

}

bool Renderer::initSceneNodes() {
	root = new SceneNode(Mesh::LoadFromMeshFile("OffsetCubeY.msh"));
	root->SetTransform(Matrix4::Translation(light->GetPosition()));
	root->SetModelScale(Vector3(100, 100, 100));
	
	SceneNode* temp = new SceneNode(Mesh::LoadFromMeshFile("OffsetCubeY.msh"));
	temp->SetTransform(Matrix4::Translation({
		heightmapSize.x,
		0, 
		heightmapSize.z
		}));
	temp->SetModelScale({ 100 ,100, 100 });
	root->AddChild(temp);

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
	DrawSceneNodeItems();
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
	Vector3 hSize = heightMap->GetHeightmapSize();
	
	modelMatrix.ToIdentity();
	modelMatrix = Matrix4::Translation({ 0,0,0 })* Matrix4::Rotation(90, { 0,1,0 }) ;
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->DrawInstanced(1);

}

void Renderer::DrawGrass() {


	Vector3 hSize = heightMap->GetHeightmapSize();

	Vector3 grassOffset = - hSize / grassDimensions;
	grassOffset.y = 1.0f;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, heightBuffer);

	compShader->Bind();
	compShader->Dispatch((unsigned int)SCALE.x, (unsigned int)SCALE.y, 1);


	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glUniform3fv(glGetUniformLocation(compShader->GetProgram(), "mapSize"), 1, (float*)&hSize);
	glUniform1f(glGetUniformLocation(compShader->GetProgram(), "t"), (float)timer->GetTotalTimeSeconds());
	glUniform2fv(glGetUniformLocation(compShader->GetProgram(), "density"), 1, (float*)&SCALE);
	glUniform1f(glGetUniformLocation(compShader->GetProgram(), "windSpeed"), (float)windSpeed);
	glUniform2fv(glGetUniformLocation(compShader->GetProgram(), "windDir"), 1, (float*)&windDir);
	glUniform3fv(glGetUniformLocation(compShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform3fv(glGetUniformLocation(compShader->GetProgram(), "grassDims"), 1, (float*)&grassDimensions);
	glUniform3fv(glGetUniformLocation(compShader->GetProgram(), "offset"), 1, (float*)&grassOffset);


	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	BindShader(gpuShader);
	SetShaderLight(*light);

	modelMatrix = Matrix4::Translation({ 0 , 0, 0}) * Matrix4::Scale(grassDimensions);
	textureMatrix.ToIdentity();

	
	
	glUniform1i(glGetUniformLocation(gpuShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(gpuShader->GetProgram(), "windTex"),	 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, compVnoise);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, windTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	glUniform1i(glGetUniformLocation(gpuShader->GetProgram(), "useTexture"), 0);


	glUniform3fv(glGetUniformLocation(gpuShader->GetProgram(), "spacePerBlade"), 1, (float*)&grassOffset);
	glUniform1f(glGetUniformLocation(gpuShader->GetProgram(), "time"), (float)timer->GetTotalTimeSeconds());
	glUniform1f(glGetUniformLocation(gpuShader->GetProgram(), "windFwdSway"), (float)windFwdSway);
	glUniform1f(glGetUniformLocation(gpuShader->GetProgram(), "windRightSway"), (float)windRightSway);
	glUniform2fv(glGetUniformLocation(gpuShader->GetProgram(), "windDir"), 1, (float*)&windDir);
	glUniform3fv(glGetUniformLocation(gpuShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	UpdateShaderMatrices();
	triangle->DrawInstanced(TOTALDISPATCH);

	modelMatrix = Matrix4::Translation({ 0, 230.0f + 100.0f,0  }) * Matrix4::Scale({100, 100, 100} ) * Matrix4::Rotation(90, {1,0,0});

	textureMatrix.ToIdentity();
	glUniform1i(glGetUniformLocation(gpuShader->GetProgram(), "useTexture"), 1);
	UpdateShaderMatrices();
	quad->Draw();
	
}