#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"
#include "GameObject.h"

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
	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	root_object = new GameObject("ROOT");
	game_objects.push_back(root_object);
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

	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	for (uint n = 0; n < game_objects.size(); n++)
	{
		game_objects[n]->Update();
	}

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

