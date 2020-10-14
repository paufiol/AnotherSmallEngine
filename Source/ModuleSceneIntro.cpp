#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "OpenGL.h"


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	
	
	/*buffer = 0;
	glGenBuffers(1, (GLuint*)&(buffer));
	glBindBuffer(GL_ARRAY_BUFFER, buffer);*/
	//glBufferData(GL_ARRAY_BUFFER, sizeof(36.f), vertices, GL_STATIC_DRAW);

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}



// Update
update_status ModuleSceneIntro::Update(float dt)
{
	GLfloat vertices[] = {
	 0.0f, 0.0f, 0.0f,
	 5.0f, 0.0f, 0.0f,
	 0.0f,  5.0f, 0.0f,

	 0.0f, 5.0f, 0.0f,
	 5.0f, 0.0f, 0.0f,
	 5.0f,  5.0f, 0.0f,

	 5.0f, 0.0f, 0.0f,
	 5.0f, 0.0f, -5.0f,
	 5.0f,  5.0f, 0.0f,

	 5.0f, 5.0f, 0.0f,
	 5.0f, 0.0f, -5.0f,
	 5.0f,  5.0f, -5.0f,

	 5.0f, 0.0f, -5.0f,
	 0.0f, 0.0f, -5.0f,
	 5.0f,  5.0f, -5.0f,

	 5.0f, 5.0f, -5.0f,
	 0.0f, 0.0f, -5.0f,
	 0.0f,  5.0f, -5.0f,

	 0.0f, 0.0f, -5.0f,
	 0.0f, 0.0f, 0.0f,
	 0.0f,  5.0f, -5.0f,

	 0.0f, 5.0f, -5.0f,
	 0.0f, 0.0f, 0.0f,
	 0.0f,  5.0f, 0.0f,

	 0.0f, 5.0f, 0.0f,
	 5.0f, 5.0f, 0.0f,
	 0.0f,  5.0f, -5.0f,

	 0.0f, 5.0f, -5.0f,
	 5.0f, 5.0f, 0.0f,
	 5.0f,  5.0f, -5.0f,
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glDrawArrays(GL_TRIANGLES, 0, 30);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Update();
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Render();
	}
	return UPDATE_CONTINUE;
}
