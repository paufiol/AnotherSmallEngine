
#pragma once
#include "Component.h"
//#include "glmath.h"

#include "Dependencies/MathGeoLib/include/MathGeoLib.h"
#include "Dependencies/MathGeoLib/include/Geometry/Frustum.h"

class ComponentCamera : public Component {
	//Properties
public:
	
	Frustum frustum;

	bool active_camera;

private:

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;
	void CleanUp() override;

	float GetFOV();
	void SetFOV(float fov);
	
	float GetAspectRatio();
	void SetAspectRatio(float AspectRatio);
	
	float GetNearPlane();
	void SetNearPlane(float distance);

	float GetFarPlane();
	void SetFarPlane(float distance);

	vec* GetFrustumPoints();

	ComponentCamera(GameObject* parent);
private:
};