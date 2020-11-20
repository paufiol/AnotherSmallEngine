#pragma once
#include "Component.h"
#include "Dependencies/MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"
#include "Globals.h"


class ComponentTransform : public Component {
	//Properties
public:


private:
	float3		position = float3(0.f, 0.f, 0.f);
	float3		scale = float3(1.f, 1.f, 1.f);
	Quat		rotation = Quat::identity;

	float3		eulerRotation = float3(0.f, 0.f, 0.f);
	float3		_eulerRotation = float3(0.f, 0.f, 0.f);

	float4x4	local_transform;
	float4x4	global_transform;

	bool		globalTransform_toUpdate = false; // To delete
	
	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;


	void inline SetPosition(float3 position) { this->position = position;  UpdateLocalTransform();};
	void inline SetRotation(Quat rotation) { this->rotation = rotation; UpdateLocalTransform();};
	void SetEulerRotation(float3 euler_angles);
	void inline SetScale(float3 scale) { this->scale = scale; UpdateLocalTransform();};

	float3 inline GetPosition() const { return this->position ; };
	Quat inline GetRotation() const { return this->rotation; };
	float3 inline GetScale() const { return this->scale; };
	float4x4 inline GetLocalTransform() const { return this->local_transform; };
	float4x4 inline GetGlobalTransform() const { return this->global_transform; };

	ComponentTransform(GameObject* parent); //DEFAULTS TO 0,0
	ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation);
	~ComponentTransform();
private:
	void UpdateLocalTransform();
	void UpdateGlobalTransform();
	void UpdateTRS();
	void UpdateEulerAngles();


	//void UpdateMatrix(); //Call after every interaction with the matrix;  
};