#pragma once

#include "Mesh.h"

#include <vector>
#include <string>

#include <assimp/material.h>
#include <MathGeoLib/src/Geometry/AABB.h>

class aiScene;

class Model
{
public:
	Model();
	~Model();

	void Load(const char* fileName);
	void LoadMaterials(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);
	void Draw();

	AABB GetAABB() const;

private:
	std::vector<unsigned> textures;
	std::vector<Mesh*> meshes;
	
	AABB aabb;
};

