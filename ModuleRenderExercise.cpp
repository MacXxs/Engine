#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "Application.h"
#include <iostream>
#include <GL/glew.h>

ModuleRenderExercise::ModuleRenderExercise() {}
ModuleRenderExercise::~ModuleRenderExercise() {}

bool ModuleRenderExercise::Init()
{
	ENGINGE_LOG("Module Render Exercise init!");

	bool ret = true;

	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	char* vertexSource = App->program->LoadShaderSource("Shaders/default_vertex.glsl");
	char* fragmentSource = App->program->LoadShaderSource("Shaders/default_fragment.glsl");

	unsigned vertexShader = App->program->CompileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned fragmentShader = App->program->CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	App->program->CreateProgram(vertexShader, fragmentShader);

	return ret;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
	update_status status = UPDATE_CONTINUE;

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glUseProgram(App->program->program);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);

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