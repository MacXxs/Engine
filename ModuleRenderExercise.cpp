#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "Application.h"
#include "ModuleDebugDraw.h"
#include "ModuleEngineCamera.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"

#include "Model.h"

//#include "debug_draw.hpp"

#include <iostream>
#include <GL/glew.h>

#include "Libraries/MathGeoLib/src/Geometry/Frustum.h"
#include "Libraries/MathGeoLib/src/Math/float3x3.h"
#include "SDL.h"

ModuleRenderExercise::ModuleRenderExercise() {}
ModuleRenderExercise::~ModuleRenderExercise() {}

bool ModuleRenderExercise::Start()
{
	ENGINE_LOG("Module Render Exercise init!");

	bool ret = true;

	char* vertexSource = App->program->LoadShaderSource("Shaders/default_vertex.glsl");
	char* fragmentSource = App->program->LoadShaderSource("Shaders/default_fragment.glsl");

	unsigned vertexShader = App->program->CompileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned fragmentShader = App->program->CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	App->program->CreateProgram(vertexShader, fragmentShader);
	
	Model* bakerHouse = new Model;
	bakerHouse->Load("Assets/models/BakerHouse.fbx");

	models.push_back(*bakerHouse);

	/*
	float vtx_data[] = {
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f,

		1.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 0.f,

		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,

		1.f, 1.f,
		0.f, 1.f,
		0.f, 0.f
	};

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	App->textures->Load("baboon.ppm");
	*/

	return ret;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
	update_status status = UPDATE_CONTINUE;

	//renderTriangle();
	
	for (auto it = models.begin(); it != models.end(); ++it)
	{
		it->Draw();
	}

	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	App->debug->Draw(App->engineCamera->GetViewMatrix(),
		App->engineCamera->GetProjectionMatrix(), w, h);

	return status;
}

update_status ModuleRenderExercise::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	bool ret = true;

	glDeleteBuffers(1, &this->vbo);

	return ret;
}

void ModuleRenderExercise::renderTriangle()
{
	int w, h;
	float4x4 model, view, proj;

	SDL_GetWindowSize(App->window->window, &w, &h);

	proj = App->engineCamera->GetProjectionMatrix();
	model = float4x4::FromTRS(float3(1.0f, 0.0f, 0.0f),
		float4x4::RotateZ(0),
		float3(1.0f, 1.0f, 0.0f));
	//view = float4x4::LookAt(float3(0, 2, 10), float3(0.0f, 0.0f, 0.0f), -float3::unitZ, float3::unitY, float3::unitY);
	view = App->engineCamera->GetViewMatrix();
	
	glUseProgram(App->program->program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glEnableVertexAttribArray(0); 
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2,
		(void*)(sizeof(float) * 3 * 6) // buffer offset
	);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, App->textures->texture);

	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 6);

	App->debug->Draw(view, proj, w, h);
}