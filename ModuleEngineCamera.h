#pragma once

#include "Module.h"
#include "ModuleDebugDraw.h"

#include "Libraries/MathGeoLib/src/Geometry/Frustum.h"
#include "Libraries/MathGeoLib/src/Math/float4x4.h"

#define DEFAULT_MOVE_SPEED 5.f
#define DEFAULT_ROTATION_DEGREE 25
#define DEFAULT_ROTATION_SPEED 2.5f
#define DEFAULT_MOUSE_SPEED_MODIFIER 10.f
#define DEFAULT_SHIFT_ACCELERATION 2.f

#define MAX_MOUSE_SPEED_MODIFIER 50.f
#define MAX_HFOV 120
#define MAX_VFOV 85

#define MIN_HFOV 60
#define MIN_VFOV 34

class ModuleEngineCamera : public Module
{
public:
	ModuleEngineCamera();
	~ModuleEngineCamera();

	bool Init() override;

	update_status Update();

	void Move();
	void Rotate();
	void Rotate(const float3x3& rotationMatrix);
	void FreeLook();
	void Run();
	void Walk();
	void Zoom();
	void Focus(const AABB &aabb);
	void Orbit(const AABB& aabb);

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
	float GetDistance(float3 point) const;

private:

	Frustum frustum;
	float3 position;
	float aspectRatio;
	float acceleration;
	float moveSpeed;
	float rotationSpeed;
	float mouseSpeedModifier;
};

