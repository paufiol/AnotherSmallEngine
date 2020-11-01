#pragma once
#include "Component.h"
#include "Dependencies/MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"


class ComponentTransform : public Component {
	//Properties
public:


private:
	float3 position;
	Quat rotation;
	float3 scale;
	float4x4 transform;
	
	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;

	void inline SetPosition(float3 position) { this->position = position;  UpdateMatrix();};
	void inline SetRotation(Quat rotation) { this->rotation = rotation; UpdateMatrix();};
	void inline SetScale(float3 scale) { this->scale = scale; UpdateMatrix();};

	float3 inline GetPosition() const { return this->position ; };
	Quat inline GetRotation() const { return this->rotation; };
	float3 inline GetScale() const { return this->scale; };
	float4x4 inline GetTransformMatrix() const { return this->transform; };

	ComponentTransform(GameObject* parent); //DEFAULTS TO 0,0
	ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation);
private:

	void UpdateMatrix(); //Call after every interaction with the matrix;  
};