#include "Application.h"
#include "ModuleEngineCamera.h"
#include "ModuleWindow.h"

#include "Libraries/MathGeoLib/src/Math/float3x3.h"
#include "Libraries/MathGeoLib/src/Math/Quat.h"

ModuleEngineCamera::ModuleEngineCamera() {};

ModuleEngineCamera::~ModuleEngineCamera() {};

bool ModuleEngineCamera::Init()
{
	int w, h;

	SDL_GetWindowSize(App->window->window, &w, &h);
	aspectRatio = float(w) / h;

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetHorizontalFovAndAspectRatio(math::DegToRad(90), aspectRatio);

	acceleration = DEFAULT_SHIFT_ACCELERATION;
	moveSpeed = DEFAULT_MOVE_SPEED;
	rotationSpeed = DEFAULT_ROTATION_SPEED;

	position = float3(0.f, 2.f, 5.f);

	frustum.SetPos(position);
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);

	return true;
}

void ModuleEngineCamera::Move(camera_movement move)
{
	switch (move)
	{
	case camera_movement::MOVE_FORWARD:
		position = position + frustum.Front().Normalized() * moveSpeed * acceleration * App->deltaTime;
		break;

	case camera_movement::MOVE_BACKWARDS:
		position = position + -(frustum.Front().Normalized()) * moveSpeed * acceleration * App->deltaTime;
		break;

	case camera_movement::MOVE_LEFT:
		position = position + -(frustum.WorldRight()) * moveSpeed * acceleration * App->deltaTime;
		break;

	case camera_movement::MOVE_RIGHT:
		position = position + frustum.WorldRight() * moveSpeed * acceleration * App->deltaTime;
		break;

	case camera_movement::MOVE_UP:
		position = position + frustum.Up() * moveSpeed * acceleration * App->deltaTime;
		break;

	case camera_movement::MOVE_DOWN:
		position = position + -(frustum.Up()) * moveSpeed * acceleration * App->deltaTime;
		break;
	}
	
	frustum.SetPos(position);
}

void ModuleEngineCamera::Rotate(camera_movement move)
{
	float yaw = 0.f, pitch = 0.f;

	float rotationAngle = RadToDeg(frustum.Front().Normalized().AngleBetween(float3::unitY));

	switch (move)
	{
	case camera_movement::ROTATE_UP:
		if (rotationAngle - rotationSpeed * acceleration > 0) pitch = 
			math::DegToRad(DEFAULT_ROTATION_DEGREE);
		break;

	case camera_movement::ROTATE_DOWN:
		if (rotationAngle + rotationSpeed * acceleration < 180) pitch =
			math::DegToRad(-DEFAULT_ROTATION_DEGREE);
		break;

	case camera_movement::ROTATE_LEFT:
		yaw = math::DegToRad(DEFAULT_ROTATION_DEGREE);
		break;

	case camera_movement::ROTATE_RIGHT:
		yaw = math::DegToRad(-DEFAULT_ROTATION_DEGREE);
		break;
	}

	Quat pitchQuat(frustum.WorldRight(), pitch * App->deltaTime * rotationSpeed * acceleration);
	Quat yawQuat(float3::unitY, yaw * App->deltaTime * rotationSpeed * acceleration);

	float3x3 rotationMatrixX = float3x3::FromQuat(pitchQuat);
	float3x3 rotationMatrixY = float3x3::FromQuat(yawQuat);
	float3x3 rotationDeltaMatrix = rotationMatrixX * rotationMatrixY;

	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationDeltaMatrix.MulDir(oldUp));

	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationDeltaMatrix.MulDir(oldFront));
}

void ModuleEngineCamera::MouseRotate(int xrel, int yrel)
{
	float yaw = 0.f, pitch = 0.f;

	float rotationAngle = RadToDeg(frustum.Front().Normalized().AngleBetween(float3::unitY));

	if (yrel > 0) { if (rotationAngle - yrel > 0) pitch = math::DegToRad(yrel); }
	else if (yrel < 0) { if (rotationAngle - yrel < 180) pitch = math::DegToRad(yrel); }
	
	yaw = math::DegToRad(-xrel);

	Quat pitchQuat(frustum.WorldRight(), pitch * App->deltaTime);
	Quat yawQuat(float3::unitY, yaw * App->deltaTime);

	float3x3 rotationMatrixX = float3x3::FromQuat(pitchQuat);
	float3x3 rotationMatrixY = float3x3::FromQuat(yawQuat);
	float3x3 rotationDeltaMatrix = rotationMatrixX * rotationMatrixY;

	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationDeltaMatrix.MulDir(oldUp));

	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationDeltaMatrix.MulDir(oldFront));
}

void ModuleEngineCamera::Run()
{
	acceleration = DEFAULT_SHIFT_ACCELERATION;
}

void ModuleEngineCamera::Walk()
{
	acceleration = 1.f;
}

void ModuleEngineCamera::SetHFOV(float fov)
{
	frustum.SetHorizontalFovAndAspectRatio(fov, aspectRatio);
}

void ModuleEngineCamera::SetVFOV(float fov)
{
	frustum.SetVerticalFovAndAspectRatio(fov, aspectRatio);
}

void ModuleEngineCamera::SetAspectRatio(float aspect)
{
	aspectRatio = aspect;
	SetHFOV(frustum.HorizontalFov());
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

void ModuleEngineCamera::SetMoveSpeed(float speed)
{
	moveSpeed = speed;
}

void ModuleEngineCamera::SetRotationSpeed(float speed)
{
	rotationSpeed = speed;
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

float ModuleEngineCamera::GetHFOV() const
{
	return math::RadToDeg(frustum.HorizontalFov());
}

float ModuleEngineCamera::GetVFOV() const
{
	return math::RadToDeg(frustum.VerticalFov());
}

float ModuleEngineCamera::GetZNear() const
{
	return frustum.NearPlaneDistance();
}

float ModuleEngineCamera::GetZFar() const
{
	return frustum.FarPlaneDistance();
}

float ModuleEngineCamera::GetMoveSpeed() const
{
	return moveSpeed;
}

float ModuleEngineCamera::GetRotationSpeed() const
{
	return rotationSpeed;
}
