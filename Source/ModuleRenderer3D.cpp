#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ImporterTexture.h"

#include "GameObject.h"

#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Dependencies/MathGeoLib/include/Geometry/Plane.h"
#include "Dependencies/MathGeoLib/include/Geometry/LineSegment.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#pragma comment (lib, "Dependencies/Glew/libx86/Release/Win32/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled), context()
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	glewInit();

	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(.05f, .07f, .07f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		lights[0].Active(true);
		SetDepthtest(false);
		SetCullface(false);
		SetLighting(false);
		SetColormaterial(false);
		SetTexture2D(false);	
		//glEnable(GL_BLEND);
		
	}	
	Importer::TextureImporter::InitDevil();

	UseCheckerTexture();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->GetRawViewMatrix());

	// light 0 on cam pos

	lights[0].SetPos(App->camera->currentCamera->frustum.Pos().x, App->camera->currentCamera->frustum.Pos().y, App->camera->currentCamera->frustum.Pos().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	//Scene Grid
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor4f(0.7f, 0.7f, 0.7f, 0.7f);
	float z = 70.0f;
	for (float x = -70; x <= z; x += 2.0f)
	{
		glVertex3f(x, -1.0f, -z);
		glVertex3f(x, -1.0f, z);
		glVertex3f(-z, -1.0f, x);
		glVertex3f(z, -1.0f, x);
	}

	if (App->camera->gameCamera != nullptr) {
		glLineWidth(2.0f);
		vec* frustum_corners;
		frustum_corners = App->camera->gameCamera->GetFrustumPoints();
		DrawCuboid(frustum_corners, App->editor->frustumColor);
	}

	glEnd();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	if (App->editor->drawWireframe) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glEnable(GL_TEXTURE_CUBE_MAP); }
	else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glDisable(GL_TEXTURE_CUBE_MAP); }
	
	IterateMeshDraw();

	ImGui::GetBackgroundDrawList();
	

	
	App->editor->DrawGUI();

	UpdateProjectionMatrix();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	//Only do if cameras are up and running
	glViewport(0, 0, width, height);
	
	if (App->camera->currentCamera != nullptr) {

		App->camera->currentCamera->SetAspectRatio((float)width / (float)height);
	}

}

void ModuleRenderer3D::UpdateProjectionMatrix()
{
	//Only do if cameras are up and running
	if (App->camera->currentCamera == nullptr) return;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf((GLfloat*)App->camera->currentCamera->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void ModuleRenderer3D::IterateMeshDraw()
{
	/*for(uint i = 0; i < Importer::MeshImporter::meshes.size(); i++)
	{
		DrawMesh(Importer::MeshImporter::meshes[i]);

		if (App->editor->drawNormals) DrawNormals(Importer::MeshImporter::meshes[i]);
		//LOG("Mesh rendered with %d vertices", Importer::MeshImporter::meshes[i]->size[Mesh::vertex]);
	}*/

	for (uint i = 0; i < App->scene->game_objects.size(); i++) 
	{
		if (!App->scene->game_objects.at(i)->active && !App->scene->game_objects.at(i)->children.empty()) break;

		if (App->scene->game_objects[i]->GetComponent(ComponentType::Mesh) != nullptr)
		{
			
			std::vector<Component*> meshComponents = App->scene->game_objects[i]->GetComponents(ComponentType::Mesh);
			std::vector<Component*>::iterator item = meshComponents.begin();

			for (; item != meshComponents.end(); ++item) {
				
				if (!App->scene->game_objects.at(i)->active) break;

				ComponentTexture* componentTex = (ComponentTexture*)App->scene->game_objects[i]->GetComponent(ComponentType::Material);
				ComponentMesh* tempComponentMesh = (ComponentMesh*)(*item);
				ComponentTransform* tempComponentTransform = (ComponentTransform*)App->scene->game_objects[i]->GetComponent(ComponentType::Transform);
				if (componentTex != nullptr) 
				{
					DrawMesh(tempComponentMesh->GetMesh(), tempComponentTransform->GetGlobalTransform(), componentTex->GetMaterial(), App->scene->game_objects[i]);
					if (App->editor->drawNormals) DrawNormals(tempComponentMesh->GetMesh(), tempComponentTransform->GetGlobalTransform());
				}
				else 
				{
					DrawMesh(tempComponentMesh->GetMesh(), tempComponentTransform->GetLocalTransform(), nullptr, App->scene->game_objects[i]);
					if (App->editor->drawNormals) DrawNormals(tempComponentMesh->GetMesh(), tempComponentTransform->GetLocalTransform());
				}

				if (drawboundingboxes) {
					glLineWidth(2.0f);
					glBegin(GL_LINES);

					vec* corners = new vec[8];
					App->scene->game_objects[i]->aabb.GetCornerPoints(corners);
					DrawCuboid(corners, App->editor->AABBColor);
					App->scene->game_objects[i]->obb.GetCornerPoints(corners);
					DrawCuboid(corners, App->editor->OBBColor);

					delete[] corners;

					glEnd();
				}
			}
		}
	}
}

void ModuleRenderer3D::DrawMesh(ResourceMesh* mesh, float4x4 transform, ResourceMaterial* rMaterial, GameObject* meshOwner)
{
	
	if (App->camera->gameCamera->frustum_culling) 
	{
		if(!DoesIntersect(meshOwner->aabb))
		{
			return;
		}
	}
	
	if (rMaterial != nullptr)
	{
		if (rMaterial->GetId() == 0)
		{
			Color color = rMaterial->GetColor();
			glColor4f(color.r, color.g, color.b, color.a);
		}
		else if (App->editor->drawTexture && !App->editor->drawCheckerTex)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, rMaterial->GetId());

		}
		else if (!App->editor->drawTexture && App->editor->drawCheckerTex)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, checkerID);
		}
	}
	
	if (!App->scene->root_object->children.empty() && mesh != nullptr)
	{
		glPushMatrix();	// Set the matrix on top of the stack identical to the one below it
		glMultMatrixf((float*)&transform.Transposed());

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[ResourceMesh::vertex]);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[ResourceMesh::normal]);
		glNormalPointer(GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ID[ResourceMesh::index]);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[ResourceMesh::texture]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glDrawElements(GL_TRIANGLES, mesh->size[ResourceMesh::index], GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_NORMAL_ARRAY, 0);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);


		glPopMatrix();	// Pops the current matrix stack, replacing the current matrix with the one below it on the stack
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		//LOG("Unable to render mesh");
	}
}

void ModuleRenderer3D::UseCheckerTexture() {

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkerID);
	glBindTexture(GL_TEXTURE_2D, checkerID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
}

void ModuleRenderer3D::DrawNormals(ResourceMesh* mesh, float4x4 transform)
{
	glBegin(GL_LINES);
	glColor4f(App->editor->NormalColor.r, App->editor->NormalColor.g, App->editor->NormalColor.b, App->editor->NormalColor.a);
	
	for (uint i = 0; i < mesh->size[ResourceMesh::vertex] * 3; i +=3)
	{
		LineSegment NormalDirection(vec(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]), 
									vec(mesh->vertices[i] + mesh->normals[i], mesh->vertices[i + 1] + mesh->normals[i + 1], mesh->vertices[i + 2] + mesh->normals[i + 2]));

		NormalDirection.Transform(transform);

		glVertex3f(NormalDirection.a.x, NormalDirection.a.y, NormalDirection.a.z);
		glVertex3f(NormalDirection.b.x, NormalDirection.b.y, NormalDirection.b.z);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
}


void ModuleRenderer3D::SetDepthtest(bool state) {
	if (state == false)
		glEnable(GL_DEPTH_TEST);
	else if (state == true)
		glDisable(GL_DEPTH_TEST);
}
void ModuleRenderer3D::SetCullface(bool state) {
	if (state == false)
		glEnable(GL_CULL_FACE);
	else if (state == true)
		glDisable(GL_CULL_FACE);
}
void ModuleRenderer3D::SetLighting(bool state) {
	if (state == false)
		glEnable(GL_LIGHTING);
	else if (state == true)
		glDisable(GL_LIGHTING);
}
void ModuleRenderer3D::SetColormaterial(bool state) {
	if (state == false)
		glEnable(GL_COLOR_MATERIAL);
	else if (state == true)
		glDisable(GL_COLOR_MATERIAL);
}
void ModuleRenderer3D::SetTexture2D(bool state) {
	if (state == false)
		glEnable(GL_TEXTURE_2D);
	else if (state == true)
		glDisable(GL_TEXTURE_2D);
}
void ModuleRenderer3D::SetCubemap(bool state) {
	if (state == false)
		glEnable(GL_TEXTURE_CUBE_MAP);
	else if (state == true)
		glDisable(GL_TEXTURE_CUBE_MAP);
}
void ModuleRenderer3D::SetPolygonssmooth(bool state) {
	if (state == false)
		glEnable(GL_POLYGON_SMOOTH);
	else if (state == true)
		glDisable(GL_POLYGON_SMOOTH);
}

bool ModuleRenderer3D::DoesIntersect(const AABB& aabb) {

	static vec aabb_corners[8];
	aabb.GetCornerPoints(aabb_corners);

	static Plane frustum_planes[6];
	App->camera->gameCamera->frustum.GetPlanes(frustum_planes);

	for(int p = 0; p < 6; p++)
	{
		int iInCount = 8;

		for(int i = 0; i < 8; i++)
		{
			if(frustum_planes[p].IsOnPositiveSide(aabb_corners[i]))
			{
				--iInCount;
			}
		}
		if (iInCount == 0) return false;
	}

	return true;
}

void ModuleRenderer3D::DrawCuboid(const float3* corners, Color color) {
	
	glColor4f(color.r, color.g, color.b, color.a);

	//Between-planes right
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[3]);

	//Between-planes left
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[2]);
	glVertex3fv((GLfloat*)&corners[6]);

	//Far plane horizontal
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[7]);

	//Near plane horizontal
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[2]);

	//Near plane vertical
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[2]);

	//Far plane vertical
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[6]);

	glColor4f(1.0, 1.0, 1.0, 1.0);

}