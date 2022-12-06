#include "Model.h"

#include "Globals.h"
#include "Application.h"

#include "ModuleTexture.h"
#include "ModuleEngineCamera.h"

#include "Mesh.h"

#include <assimp/scene.h>       
#include <assimp/postprocess.h> 
#include <assimp/cimport.h>
#include <assimp/types.h>

Model::Model()
{
	aabb.SetNegativeInfinity();
}

Model::~Model()
{
	meshes.clear();
}

void Model::Load(const char* fileName)
{
	this->path = fileName;

	ENGINE_LOG("Model: %s", fileName);

	const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		// TODO: LoadTextures(scene->mMaterials, scene->mNumMaterials);
		// TODO: LoadMeshes(scene->mMeshes, scene->mNumMeshes);
		LoadMaterials(scene);
		LoadMeshes(scene);

		App->engineCamera->Focus(this->aabb);
	}
	else
	{
		ENGINE_LOG("Error loading %s: %s", fileName, aiGetErrorString());
	}
}

void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;

	textures.reserve(scene->mNumMaterials);

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			textures.push_back(App->textures->Load(file.data, GetDirectory()));
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	meshes.reserve(scene->mNumMeshes);

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		Mesh* mesh = new Mesh(scene->mMeshes[i]);

		aabb.Enclose(mesh->GetVertices(), mesh->GetNumVertices());

		meshes.push_back(mesh);
	}
}

void Model::Draw()
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		meshes[i]->Draw(textures);
	}
}

AABB Model::GetAABB() const
{
	return aabb;
}

const std::string Model::GetDirectory() const
{
	std::string directory = this->path;
	bool found = false;

	int i = directory.size() - 1;
	for (; !found && i > 0; --i)
	{
		if (directory[i] == '\\') found = true;
	}

	if (true)
	{
		directory = directory.substr(0, i+2);
	}

	return directory;
}
