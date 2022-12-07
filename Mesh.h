#pragma once

#include <vector>

#include <assimp/mesh.h>
#include <MathGeoLib/src/Math/float3.h>
#include <MathGeoLib/src/Math/float4x4.h>

class Mesh
{
public:
	Mesh(const aiMesh* mesh);
	~Mesh();

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& model_textures);

	vec* GetVertices() const;
	unsigned GetNumVertices() const;
	unsigned GetNumTriangles() const;
	
private:
	unsigned vbo;
	unsigned ebo;
	unsigned vao;
	
	unsigned materialIndex;
	unsigned numVertices;
	unsigned numIndexes;

	float4x4 modelTransform = float4x4::identity;

	vec* vertices;
};

