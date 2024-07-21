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
#include "imgui.h"
#include "imgui_impl_opengl3.h"

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


	Vector2 SCALE = { 0,0 };
	int		TOTALDISPATCH = 0;
	Vector3 heightmapSize;

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
	Mesh* grassMesh;

	Light* light;
	Camera* camera;
	GameTimer* timer;

	Vector3 localOrigin;

	GLuint cubeMap		= 0;
	GLuint waterTex		= 0;
	GLuint earthTex		= 0;
	GLuint debugTex		= 0;
	GLuint earthBump	= 0;
	GLuint grassTex		= 0;
	GLuint compVnoise	= 0;
	GLuint windTex		= 0;

	GLuint ssboID	=0;
	GLuint heightBuffer = 0;

	float windSpeed;
	float windFwdSway;
	float windRightSway;
	Vector2 windDir;
	Vector3 grassDimensions;
};

