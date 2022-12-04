#pragma once

#include <vector>

#include <assimp/mesh.h>
#include <MathGeoLib/src/Math/float3.h>

class Mesh
{
public:
	Mesh(const aiMesh* mesh);
	~Mesh() {};

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& model_textures);

	vec* GetVertices() const;
	unsigned GetNumVertices() const;
	
private:
	unsigned vbo;
	unsigned ebo;
	unsigned vao;
	
	unsigned materialIndex;
	unsigned numVertices;
	unsigned numIndexes;

	vec* vertices;
};

