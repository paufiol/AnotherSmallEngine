#pragma once
#include "ComponentCamera.h"
#include "GameObject.h"

#include "Dependencies/ImGUI/imgui.h"

ComponentCamera::ComponentCamera(GameObject* owner) : active_camera(false), Component(owner)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 1000.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	type = ComponentType::Camera;
}

void ComponentCamera::Enable() {

}

void ComponentCamera::Disable() {

}

void ComponentCamera::Update() {

}

void ComponentCamera::CleanUp()
{

}

void ComponentCamera::DrawInspector() {

	if (ImGui::CollapsingHeader("Component Camera"), ImGuiTreeNodeFlags_DefaultOpen)
	{


	}
}
