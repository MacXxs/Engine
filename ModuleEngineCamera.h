#pragma once

#include "Module.h"
#include "ModuleDebugDraw.h"

#include "Libraries/MathGeoLib/src/Geometry/Frustum.h"
#include "Libraries/MathGeoLib/src/Math/float4x4.h"

#define DEFAULT_MOVE_SPEED 0.05f
#define DEFAULT_ROTATION_SPEED 0.8f

#define SHIFT_ACCELERATION 2.f
#define WHEEL_ACCELERATION 10.f

#define ROTATION_DEGREE 1

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

#pragma region interaction
	void Move(camera_movement move);
	void Rotate(camera_movement move);
	void MouseRotate(int xrel, int yrel);
	void Run(float acceleration);
#pragma endregion interaction

#pragma region setters
	void SetHFOV(float fov);
	void SetVFOV(float fov);
	void SetAspectRatio(float aspect);
	void SetPlaneDistance(float zNear, float zFar);
	void SetPosition(float3 position);
	void SetOrientation(float3 orientation);
	void SetLookAt(float3 lookAt);
#pragma endregion setters

#pragma region getters
	float4x4 GetProjectionMatrix() const;
	float4x4 GetViewMatrix();

	float GetHFOV();
	float GetVFOV();
#pragma endregion getters

private:

	Frustum frustum;
	float3 position;
	float aspectRatio;
	float moveSpeed;
	float rotationSpeed;
};

