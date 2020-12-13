#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "OpenGL.h"
#include "Dependencies/SDL/include/SDL.h"

//#include "Dependencies/MathGeoLib/include/MathGeoLib.h"
#include "Dependencies/MathGeoLib/include/Math/float4x4.h"

#define MAX_LIGHTS 8

class ComponentCamera;
class ResourceMesh;
class ResourceMaterial;
class GameObject;

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
	void IterateMeshDraw();
	void DrawMesh(ResourceMesh* mesh, float4x4 transform, ResourceMaterial* rMaterial = nullptr, GameObject* meshOwner = nullptr);

	void DrawNormals(ResourceMesh* mesh, float4x4 transform);

	void SetDepthtest(bool active);
	void SetCullface(bool active);
	void SetLighting(bool active);
	void SetColormaterial(bool active);
	void SetTexture2D(bool active);
	void SetCubemap(bool active);
	void SetPolygonssmooth(bool active);

	void UpdateProjectionMatrix();

private:
	bool DoesIntersect(const AABB& aabb);

	void DrawCuboid(const float3* corners, Color color);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	GLbyte checkerImage[64][64][4];
	GLuint checkerID;

	ResourceMesh* mesh = nullptr;
	GLuint newTexture = 0;

	bool drawboundingboxes = false;
};