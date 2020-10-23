#pragma once

#include <vector>
#include <string>
#include "Globals.h"
#include "Timer.h"


class Module;												
class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleEditor;
class ModuleCamera3D;
class ModuleFileSystem;



class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleFileSystem* file_system ;


	bool debug;
	bool renderPrimitives;
private:

	Timer	ms_timer;
	float	dt;
	std::vector<Module*> modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* module);
	void PrepareUpdate();
	void FinishUpdate();
};