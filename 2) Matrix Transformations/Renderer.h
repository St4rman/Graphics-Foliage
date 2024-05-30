#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float dt);
	virtual void RenderScene();

	void SwitchToPerspective();
	void SwitchToOrthographic();
	void IncreaseFov();
	void DecreaseFov();

	inline void SetScale(float s) { scale = s; }
	inline void SetRotation(float r) { rotation = r; }
	inline void SetPosition(Vector3 p) { position = p; }
	inline void SetFov(float fovin) { fov = fovin; }

protected:
	Camera* camera;
	Mesh* tri;
	Shader* matrixShader;
	float scale;
	float rotation;
	Vector3 position;
	float fov;
};