#pragma once

#include "Application.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "Globals.h"
#include "ModuleWindow.h"

#include "DrawPrism.h"
#include "glmath.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"

#include "Dependencies/ImGUI/imgui.h"

ComponentCamera::ComponentCamera(GameObject* owner) : active_camera(false), Component(owner)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 10, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetPerspective(1.0f, 1.0f);
	SetFOV(80.0f);

	type = ComponentType::Camera;
}

void ComponentCamera::Enable() {}

void ComponentCamera::Disable() {}

void ComponentCamera::Update() {}

void ComponentCamera::CleanUp() {}

float ComponentCamera::GetFOV() 
{
	return frustum.VerticalFov() * RADTODEG;
}

float ComponentCamera::GetAspectRatio()
{
	return frustum.AspectRatio();
}

void ComponentCamera::SetFOV(float fov) 
{
	frustum.SetVerticalFovAndAspectRatio(fov * DEGTORAD, ((float)App->window->Width() / (float)App->window->Height()));
}

void ComponentCamera::SetAspectRatio(float aspectratio) {
	
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspectratio);
}

float ComponentCamera::GetNearPlane()
{
	return frustum.NearPlaneDistance();
}

void ComponentCamera::SetNearPlane(float distance)
{
	frustum.SetViewPlaneDistances(distance, frustum.FarPlaneDistance());
}

float ComponentCamera::GetFarPlane()
{
	return frustum.FarPlaneDistance();
}

void ComponentCamera::SetFarPlane(float distance)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), distance);
}

vec* ComponentCamera::GetFrustumPoints()
{
	static vec frustum_corners[8];
	frustum.GetCornerPoints(frustum_corners);
	
	return frustum_corners;
}

void ComponentCamera::DrawInspector() 
{

	if (ImGui::CollapsingHeader("Component Camera"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		if (ImGui::Checkbox("Viewport Camera", &active_camera)) {}
		
		//Set FOV
		float Inspector_FOV = GetFOV();
		if (ImGui::SliderFloat("FOV", &Inspector_FOV, 30, 120, "%0.2f", ImGuiSliderFlags_None)) { SetFOV(Inspector_FOV); }
		
		//Set NearPlane
		float NearPlane = GetNearPlane(); 
		if (ImGui::DragFloat("Near plane", &NearPlane)) { SetNearPlane(NearPlane); }
		
		//Set FarPlane
		float FarPlane = GetFarPlane();
		if (ImGui::DragFloat("Far plane", &FarPlane)) { SetFarPlane(FarPlane); }
	}
}
