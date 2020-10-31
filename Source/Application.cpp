
#include <list>
#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleFileSystem.h"

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	editor = new ModuleEditor();
	file_system = new ModuleFileSystem();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules

	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(file_system);

	// Scenes
	AddModule(scene_intro);
	AddModule(editor);
	// Renderer last!

	AddModule(renderer3D);
}

Application::~Application()
{
	std::vector<Module*>::iterator item = modules.begin();

	for (; item != modules.begin(); --item) 
	{
		delete (*item);
	}
}

bool Application::Init()
{
	bool ret = true;
	App = this;

	// Call Init() in all modules
	std::vector<Module*>::iterator item = modules.begin();

	for (; item != modules.end() && ret == true; ++item) {
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = modules.begin();

	for (; item != modules.end() && ret == true; ++item) {
		ret = (*item)->Start();
	}
	
	frame_time.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = (float)frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	uint last_frame_ms = frame_time.Read();
	uint frames_on_last_update = prev_last_sec_frame_count;

	fpsVec.push_back(frames_on_last_update);
	if (fpsVec.size() > totalBars)
		fpsVec.erase(fpsVec.begin());

	msVec.push_back(last_frame_ms);
	if (msVec.size() > totalBars)
		msVec.erase(msVec.begin());

	if (framerateCap > 0)
		capped_ms = 1000 / framerateCap;
	else
		capped_ms = 0;

	if (capped_ms > 0 && last_frame_ms < capped_ms)
		SDL_Delay(capped_ms - last_frame_ms);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::vector<Module*>::iterator item = modules.begin();
	
for(; item != modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->PreUpdate(dt);
	}

	item = modules.begin();
	
	for(;item != modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->Update(dt);
	}

	item = modules.begin();
	
	for(;item != modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->PostUpdate(dt);
	}
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (auto item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		ret = (*item)->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* module)
{
	modules.push_back(module);
}

Application* App = nullptr;

