#pragma once

#include "Module.h"
#include "ModuleDebugDraw.h"

#include "Libraries/MathGeoLib/src/Geometry/Frustum.h"
#include "Libraries/MathGeoLib/src/Math/float4x4.h"

#define DEFAULT_MOVE_SPEED 0.05f
#define DEFAULT_ROTATION_SPEED 0.8f
#define DEFAULT_SHIFT_ACCELERATION 2.f
#define DEFAULT_WHEEL_ACCELERATION 10.f
#define DEFAULT_ROTATION_DEGREE 1

enum class camera_movement { 
	MOVE_FORWARD,	MOVE_BACKWARDS, 
	MOVE_LEFT,		MOVE_RIGHT,
	MOVE_UP,		MOVE_DOWN,
	ROTATE_UP,		ROTATE_DOWN,
	ROTATE_LEFT,	ROTATE_RIGHT
};

class ModuleEngineCamera : public Module
{
public:
	ModuleEngineCamera();
	~ModuleEngineCamera();

	bool Init() override;

	void Move(camera_movement move);
	void Rotate(camera_movement move);
	void MouseRotate(int xrel, int yrel);
	void Run();
	void Walk();

	void SetHFOV(float fov);
	void SetVFOV(float fov);
	void SetAspectRatio(float aspect);
	void SetPlaneDistance(float zNear, float zFar);
	void SetPosition(float3 position);
	void SetOrientation(float3 orientation);
	void SetLookAt(float3 lookAt);
	void SetMoveSpeed(float speed);
	void SetRotationSpeed(float speed);

	float4x4 GetProjectionMatrix() const;
	float4x4 GetViewMatrix() const;
	float GetHFOV() const;
	float GetVFOV() const;
	float GetZNear() const;
	float GetZFar() const;
	float GetMoveSpeed() const;
	float GetRotationSpeed() const;

private:

	Frustum frustum;
	float3 position;
	float aspectRatio;
	float acceleration;
	float moveSpeed;
	float rotationSpeed;
};

