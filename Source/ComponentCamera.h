
#pragma once
#include "Component.h"
//#include "glmath.h"

//#include "Dependencies/MathGeoLib/include/MathGeoLib.h"
#include "Dependencies/MathGeoLib/include/Math/float4x4.h"
#include "Dependencies/MathGeoLib/include/Geometry/Frustum.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Geometry/LineSegment.h"

class ComponentCamera : public Component {
	//Properties
public:
	
	Frustum frustum;

	bool active_camera;
	bool frustum_culling;
	bool draw_boundingboxes;

private:
	bool looking; 
	float3 looking_at;

	LineSegment latest_ray;

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;
	void CleanUp() override;

	float GetFOV() const;
	void SetFOV(float fov);
	
	float GetAspectRatio() const;
	void SetAspectRatio(float AspectRatio);
	
	float GetNearPlane() const;
	void SetNearPlane(float distance);

	float GetFarPlane() const;
	void SetFarPlane(float distance);

	vec* GetFrustumPoints() const;

	void OnUpdateTransform(const float4x4& global, const float4x4& parent_global) override;

	float* GetRawViewMatrix();
	mat4x4 GetViewMatrix();
	float* GetProjectionMatrix();
	
	void Look(float3& reference);
	void Match(ComponentCamera* reference);
	
	void Pan(float motion_x, float motion_y);
	void Orbit(float motion_x, float motion_y);
	void Zoom(float motion_z);
	void LookAt(float motion_x, float motion_y);

	void OnClick(float pos_x, float pos_y);

	ComponentCamera(GameObject* parent);
private:

};