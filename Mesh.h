#pragma once

#include <vector>

#include <assimp/mesh.h>

class Mesh
{
public:
	Mesh(const aiMesh* mesh);
	~Mesh() {};

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& model_textures);
	
private:
	unsigned vbo;
	unsigned ebo;
	unsigned vao;
	
	unsigned materialIndex;
	unsigned numVertices;
	unsigned numIndexes;
};

