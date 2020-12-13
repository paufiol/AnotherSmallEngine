#pragma once
#include "Component.h"
#include "glmath.h"
#include "Globals.h"

class ResourceMaterial;


class ComponentTexture : public Component {
	//Properties
public:

private:
	ResourceMaterial* rMaterial = nullptr;

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override; 
	void CleanUp() override;

	inline ResourceMaterial* GetMaterial() const { return rMaterial; }
	inline void SetMaterial(ResourceMaterial* resourceMaterial) { rMaterial = resourceMaterial; }
	//inline uint32 GetResourceUID()const { return rMaterial->UID; }
	ComponentTexture(GameObject* parent); //DEFAULTS TO 0,0
	ComponentTexture(GameObject* parent, const char* texturePath, uint id);
	ComponentTexture(GameObject* parent, ResourceMaterial* rMaterial);
	//ComponentTexture(GameObject* parent, vec3 position, vec3 rotation, vec3 scale);
private:
	
};