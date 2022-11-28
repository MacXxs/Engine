#include "Model.h"

#include "Globals.h"
#include "Application.h"

#include "ModuleTexture.h"

#include "Mesh.h"

#include <assimp/scene.h>       
#include <assimp/postprocess.h> 
#include <assimp/cimport.h>
#include <assimp/types.h>

Model::Model()
{
}

Model::~Model()
{
}

void Model::Load(const char* fileName)
{
	const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		// TODO: LoadTextures(scene->mMaterials, scene->mNumMaterials);
		// TODO: LoadMeshes(scene->mMeshes, scene->mNumMeshes);
		LoadMaterials(scene);
		LoadMeshes(scene);
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
			textures.push_back(App->textures->Load(file.data));
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	meshes.reserve(scene->mNumMeshes);

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		Mesh* mesh = new Mesh(scene->mMeshes[i]);

		meshes.push_back(*mesh);
	}
}