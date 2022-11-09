#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "Application.h"
#include "ModuleDebugDraw.h"
#include "ModuleWindow.h"
#define DEBUG_DRAW_IMPLEMENTATION
#include "debug_draw.hpp"
#include <iostream>
#include <GL/glew.h>
#include "Libraries/MathGeoLib/src/Geometry/Frustum.h"
#include "Libraries/MathGeoLib/src/Math/float3x3.h"
#include "SDL.h"

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

	renderTriangle();

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
	float4x4 model, view, proj;
	
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	float aspectRatio = float(w) / h;
	float verticalFov = math::pi / 4.0f;
	float horizontalFov = 2.f * atanf(tanf(verticalFov * 0.5f) * aspectRatio);


	Frustum frustum;
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	frustum.SetPos(float3(0, 2, 10));
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	
	frustum.SetHorizontalFovAndAspectRatio(horizontalFov, aspectRatio);

	proj = frustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
		float4x4::RotateZ(pi / 4.0f),
		float3(2.0f, 1.0f, 0.0f));
	//view = float4x4::LookAt(float3(0, 2, 10), float3(0.0f, 0.0f, 0.0f), -float3::unitZ, float3::unitY, float3::unitY);
	view = frustum.ViewMatrix();
	
	glUniformMatrix4fv(0, 1, GL_TRUE, &proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glEnableVertexAttribArray(0); 
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glUseProgram(App->program->program);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);

	const ddMat4x4 transform = { // The identity matrix
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
	};

	dd::axisTriad(transform, 0.1f, 1.0f);
	dd::xzSquareGrid(-50, 50, 0.0f, 1.0f, dd::colors::Gray);

	App->debug->Draw(view, proj, w, h);
}