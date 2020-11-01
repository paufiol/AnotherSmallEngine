#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"

#include "GameObject.h"

#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleSceneIntro.h"

#include "ModuleImporter.h"

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
		glClearColor(0.f, 0.f, 0.f, 1.f);

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
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		//glEnable(GL_TEXTURE_CUBE_MAP);
	}	
	Importer::TextureImporter::InitDevil();
	UseCheckerTexture();

	//Importer::TextureImporter::Import("Assets/Textures/BakerHouse.png");
	Importer::MeshImporter::Import("Assets/Models/BakerHouse.FBX");

	ComponentTexture* tempCompTex = new ComponentTexture(App->scene_intro->selected_object);
	tempCompTex->SetTexture(Importer::TextureImporter::Import("Assets/Textures/BakerHouse.png"), "Assets/Textures/BakerHouse.png");

	App->scene_intro->selected_object->AddComponent(tempCompTex);

	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);
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
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	if (App->editor->drawWireframe) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glEnable(GL_TEXTURE_CUBE_MAP); }
	else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glDisable(GL_TEXTURE_CUBE_MAP); }
	
	IterateMeshDraw();

	App->editor->DrawGUI();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	Importer::MeshImporter::meshes.clear();
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::SetUpBuffers(Mesh* mesh)
{
	glGenBuffers(1, (GLuint*)&mesh->ID[Mesh::vertex]);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[Mesh::vertex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->size[Mesh::vertex] * 3, mesh->vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&mesh->ID[Mesh::index]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ID[Mesh::index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->size[Mesh::index], mesh->indices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&mesh->ID[Mesh::normal]);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[Mesh::normal]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh->size[Mesh::normal] * 3, mesh->normals, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&mesh->ID[Mesh::texture]);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[Mesh::texture]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->size[Mesh::texture] * 2, mesh->texCoords, GL_STATIC_DRAW);
}

void ModuleRenderer3D::IterateMeshDraw()
{
	/*for(uint i = 0; i < Importer::MeshImporter::meshes.size(); i++)
	{
		DrawMesh(Importer::MeshImporter::meshes[i]);

		if (App->editor->drawNormals) DrawNormals(Importer::MeshImporter::meshes[i]);
		//LOG("Mesh rendered with %d vertices", Importer::MeshImporter::meshes[i]->size[Mesh::vertex]);
	}*/

	for (uint i = 0; i < App->scene_intro->game_objects.size(); i++) 
	{
		if (App->scene_intro->game_objects[i]->GetComponent(ComponentType::Mesh) != nullptr)
		{
			
			std::vector<Component*> meshComponents = App->scene_intro->game_objects[i]->GetComponents(ComponentType::Mesh);
			
			std::vector<Component*>::iterator item = meshComponents.begin();
			for (; item != meshComponents.end(); ++item) {
				
				ComponentTexture* componentTex = (ComponentTexture*)App->scene_intro->game_objects[i]->GetComponent(ComponentType::Material);
				ComponentMesh* tempComponentMesh = (ComponentMesh*)(*item);
				if (componentTex != nullptr) 
				{
					DrawMesh(tempComponentMesh->GetMesh(), componentTex->GetTexture()->id);
					if (App->editor->drawNormals) DrawNormals(tempComponentMesh->GetMesh());
				}
				else 
				{
					DrawMesh(tempComponentMesh->GetMesh());
					if (App->editor->drawNormals) DrawNormals(tempComponentMesh->GetMesh());
				}
			}

		}
	
	}

}

void ModuleRenderer3D::DrawMesh(Mesh* mesh, uint id)
{
	
	if (App->editor->drawTexture && !App->editor->drawCheckerTex)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else if (!App->editor->drawTexture && App->editor->drawCheckerTex)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, checkerID);
	}
	
	if (!Importer::MeshImporter::meshes.empty())
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[Mesh::vertex]);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[Mesh::normal]);
		glNormalPointer(GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ID[Mesh::index]);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->ID[Mesh::texture]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glDrawElements(GL_TRIANGLES, mesh->size[Mesh::index], GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_NORMAL_ARRAY, 0);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);

	}
	else
	{
		LOG("Unable to render meshes (No meshes loaded)");
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

void ModuleRenderer3D::DrawNormals(Mesh* mesh)
{
	glBegin(GL_LINES);
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

	for (uint i = 0; i < mesh->size[Mesh::vertex] * 3; i +=3)
	{
	    glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
	    glVertex3f(mesh->vertices[i] + mesh->normals[i], mesh->vertices[i + 1] + mesh->normals[i + 1], mesh->vertices[i + 2] + mesh->normals[i + 2]);
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