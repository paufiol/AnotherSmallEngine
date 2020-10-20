#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class Shaders;
class I_Model;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(int width, int height);

	void LoadModel(const char* path);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	//OpenGL Context
//const int GL_VERSION_MAJOR;
//const int GL_VERSION_MINOR;

	std::vector<Shaders> shaders;
	std::vector<Texture*> textures;
	std::vector<I_Model*> models;

	I_Model* tempModel;
};