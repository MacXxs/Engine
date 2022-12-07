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

	const std::string GetDirectory() const;

	AABB GetAABB() const;
	int GetNumVertices() const;
	int GetNumTriangles() const;
	unsigned GetTextureId() const;

private:
	const char* path;

	std::vector<unsigned> textures;
	std::vector<Mesh*> meshes;
	
	AABB aabb;
};

