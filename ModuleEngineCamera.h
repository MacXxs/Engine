#pragma once

#include "Module.h"
#include "ModuleDebugDraw.h"

#include "Libraries/MathGeoLib/src/Geometry/Frustum.h"
#include "Libraries/MathGeoLib/src/Math/float4x4.h"

#define DEFAULT_SPEED 0.05f
#define SHIFT_ACCELERATION 2.f
#define WHEEL_ACCELERATION 10.f

enum class camera_movement { 
	MOVE_FORWARD, MOVE_BACKWARDS, 
	MOVE_LEFT,    MOVE_RIGHT,
	MOVE_UP,	  MOVE_DOWN
};

class ModuleEngineCamera : public Module
{
public:
	ModuleEngineCamera();
	~ModuleEngineCamera();

	bool Init() override;

	void Move(camera_movement move);
	void Run(float acceleration);

#pragma region setters
	void SetFOV(float fov);
	void SetAspectRatio(float aspect);
	void SetPlaneDistance(float zNear, float zFar);
	void SetPosition(float3 position);
	void SetOrientation(float3 orientation);
	void SetLookAt(float3 lookAt);
#pragma endregion setters

#pragma region getters
	float4x4 GetProjectionMatrix() const;
	float4x4 GetViewMatrix() const;
#pragma endregion getters

private:

	Frustum frustum;
	float3 position;
	float aspectRatio;
	float speed;
};

