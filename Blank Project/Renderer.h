#pragma once
#include "Renderer.h"
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Shader.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/CubeRobot.h"
#include "../nclgl/ComputeShader.h"
#include <algorithm>

class Camera;
class SceneNode;
class Shader;
class HeightMap;


class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene() override;
	void UpdateScene(float dt) override;

	

protected:

	bool initTextures();
	bool initShaders();
	bool initMeshes();
	bool initSceneNodes();
	bool initComputeShaders();


	void DrawHeightMap();
	void DrawWater();
	void DrawSkybox();
	void DrawSceneNodeItems();
	void DrawGrass();

	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void ClearNodeLists();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* sceneShader;
	Shader* gpuShader;

	ComputeShader* compShader;


	Frustum frameFrustum;

	vector<SceneNode*> transperentNodeList;
	vector<SceneNode*> nodeList;

	HeightMap* heightMap;
	SceneNode* root;
	Mesh* quad;
	Mesh* triangle;

	Light* light;
	Camera* camera;
	GameTimer* timer;

	Vector3 localOrigin;

	GLuint cubeMap = 0;
	GLuint waterTex = 0;
	GLuint earthTex = 0;
	GLuint debugTex = 0;
	GLuint earthBump = 0;
	GLuint grassTex = 0;

	GLuint ssboID	=0;
	GLuint ssboGB = 0;
};

