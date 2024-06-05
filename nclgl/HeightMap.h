#pragma once
#include <string>
#include "Mesh.h"
#include <iostream>

class HeightMap : public Mesh{
public: 
	HeightMap(const std::string& name, Vector3 vs);
	~HeightMap(void) {};

	Vector3 GetHeightmapSize() const { return heightmapSize; }
	bool    loadSuccess()      const { return init; }
protected:
	Vector3 heightmapSize;
	bool	init;
};

