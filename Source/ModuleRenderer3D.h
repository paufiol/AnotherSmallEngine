#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "OpenGL.h"
#include "Timer.h"
#include <map>
#include <vector>
#include "Dependencies/SDL/include/SDL.h"
#include "Dependencies/MathGeoLib/include/Math/float4x4.h"
#include "Dependencies/MathGeoLib/include/Math/float3x3.h"

#define MAX_LIGHTS 8

class ComponentCamera;
class ComponentMesh;
class ComponentMaterial;
class GameObject;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(int width, int height);

	void CreateSkybox();
	uint32 SetSkyboxShader();
	void CreateSkyboxBuffers();

	void DrawSkybox();
	void UseCheckerTexture();
	void IterateMeshDraw();
	void DrawMesh(ComponentMesh* mesh, float4x4 transform, ComponentMaterial* rMaterial = nullptr, GameObject* meshOwner = nullptr);

	void DrawNormals(ComponentMesh* mesh, float4x4 transform);

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
	uint32 SetDefaultShader(ComponentMaterial* componentMaterial);
public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	GLbyte checkerImage[64][64][4];
	GLuint checkerID;

	GLuint newTexture = 0;



	bool drawboundingboxes = false;

	Timer timer;

	std::map<float, GameObject*> sortedGO;

private:
	GLuint SkyboxTex_id = 0; //Initialise?
	std::vector<std::string> faces
	{
			"s_left.dds",
			"s_right.dds",
			"s_up.dds",
			"s_down.dds",
			"s_front.dds",
			"s_back.dds"
	}; //Add proper names

	uint Skybox_VAO = 0;
	uint Skybox_id = 0;
	uint32 Skybox_programid = 0;

	float Skybox_vertices[108] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
};