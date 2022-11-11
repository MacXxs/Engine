#include "Application.h"
#include "ModuleEngineCamera.h"
#include "ModuleWindow.h"

ModuleEngineCamera::ModuleEngineCamera()
{
};

ModuleEngineCamera::~ModuleEngineCamera() 
{
};

bool ModuleEngineCamera::Init()
{
	bool ret = true;
	int w, h;

	SDL_GetWindowSize(App->window->window, &w, &h);
	aspectRatio = float(w) / h;

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetHorizontalFovAndAspectRatio(math::DegToRad(90), aspectRatio);

	speed = DEFAULT_SPEED;
	position = float3(0.f, 2.f, 5.f);

	frustum.SetPos(position);
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);

	return ret;
}

void ModuleEngineCamera::Move(camera_movement move)
{
	switch (move)
	{
	case camera_movement::MOVE_FORWARD:
		position = position + frustum.Front().Normalized() * speed;
		break;

	case camera_movement::MOVE_BACKWARDS:
		position = position + -(frustum.Front().Normalized()) * speed;
		break;

	case camera_movement::MOVE_LEFT:
		position = position + -(frustum.WorldRight()) * speed;
		break;

	case camera_movement::MOVE_RIGHT:
		position = position + frustum.WorldRight() * speed;
		break;

	case camera_movement::MOVE_UP:
		position = position + frustum.Up() * speed;
		break;

	case camera_movement::MOVE_DOWN:
		position = position + -(frustum.Up()) * speed;
		break;
	}
	
	speed = DEFAULT_SPEED;
	frustum.SetPos(position);
}

void ModuleEngineCamera::Run(float acceleration)
{
	if (speed == DEFAULT_SPEED)
	{
		speed *= acceleration;
	}
}

void ModuleEngineCamera::SetFOV(float fov)
{
	frustum.SetHorizontalFovAndAspectRatio(fov, aspectRatio);
}

void ModuleEngineCamera::SetAspectRatio(float aspect)
{
	aspectRatio = aspect;
}

void ModuleEngineCamera::SetPlaneDistance(float zNear, float zFar)
{
	frustum.SetViewPlaneDistances(zNear, zFar);
}

void ModuleEngineCamera::SetPosition(float3 position)
{
	frustum.SetPos(position);
}

void ModuleEngineCamera::SetOrientation(float3 orientation)
{
	frustum.SetUp(orientation);
}

float4x4 ModuleEngineCamera::GetProjectionMatrix() const
{
	//return frustum.ProjectionMatrix().Transposed(); // Needs to be transposed to use withGL
	return frustum.ProjectionMatrix();
}

float4x4 ModuleEngineCamera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}
