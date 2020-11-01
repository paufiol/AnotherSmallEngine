#pragma once
#include "Component.h"
#include "glmath.h"
#include "Globals.h"

struct Texture;


class ComponentTexture : public Component {
	//Properties
public:

private:
	Texture* texture;

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override; 

	void SetTexture(Texture* texture);
	void SetTexture(uint id, const char* path);
	Texture* GetTexture();

	ComponentTexture(GameObject* parent); //DEFAULTS TO 0,0
	//ComponentTexture(GameObject* parent, vec3 position, vec3 rotation, vec3 scale);
private:
	
};