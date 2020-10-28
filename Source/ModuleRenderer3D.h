#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ResourceMesh;
class ResourceModel;

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

	void DrawAllModels();
	void LoadModel(const char* path);

	void SetDepthtest(bool active);
	void SetCullface(bool active);
	void SetLighting(bool active);
	void SetColormaterial(bool active);
	void SetTexture2D(bool active);
	void SetCubemap(bool active);
	void SetPolygonssmooth(bool active);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	std::vector<ResourceModel*> models;

};