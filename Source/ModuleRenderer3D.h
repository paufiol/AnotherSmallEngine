#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "OpenGL.h"
#include "Dependencies/SDL/include/SDL.h"

#define MAX_LIGHTS 8


struct Mesh;

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

	void UseCheckerTexture();
	void SetUpBuffers(Mesh* mesh);
	void IterateMeshDraw();
	void DrawMesh(Mesh* mesh, uint id = 0);

	void DrawNormals(Mesh* mesh);

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

	GLbyte checkerImage[64][64][4];
	GLuint checkerID;

	Mesh* mesh = nullptr;
	GLuint newTexture = 0;

};