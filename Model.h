#pragma once

#include "Mesh.h"

#include <vector>

#include <assimp/material.h>

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

private:
	std::vector<unsigned> textures;
	std::vector<Mesh> meshes;
};

