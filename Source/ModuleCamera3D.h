#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include "Dependencies/MathGeoLib/include/Geometry/Frustum.h"
#include "Dependencies/MathGeoLib/include/Math/float4x4.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Math/float2.h"

class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt) override;
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetRawViewMatrix();
	mat4x4 GetViewMatrix();
	float* GetProjectionMatrix();

	void FocusObject();

private:

	void CalculateViewMatrix();

public:
	
	ComponentCamera* editorCamera; 
	ComponentCamera* gameCamera;

	ComponentCamera* currentCamera;

	float3	reference; 

private:


	mat4x4 ViewMatrix, ViewMatrixInverse;
};