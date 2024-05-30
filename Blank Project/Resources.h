#pragma once
#include "Renderer.h"
#include "../nclgl/OGLRenderer.h"
#include <unordered_map>

class Resources
{
public: 
	Resources() {};
	void LoadTexture(std::string &name);
	GLuint* GetTexture(const std::string &name);

private:
	std::unordered_map < std::string, std::unique_ptr<GLuint>> textures;
};

