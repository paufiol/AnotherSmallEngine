#ifndef __Application__
#define __Application__

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


	bool debug = false;
	bool renderPrimitives = false;
private:
	std::vector<Module*> modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	uint					frame_count = 0;
	Timer					startup_time;
	Timer					frame_time;
	Timer					last_sec_frame_time;
	uint					last_sec_frame_count = 0;
	uint					prev_last_sec_frame_count = 0;
	uint					framerate_cap = 0;
	int						capped_ms = -1;
	int						framerateCap = 60; // Max amount of FPS
	int						totalBars = 100; // Number of bars that appear in the histogram
	std::vector<float> fpsVec;
	std::vector<float> msVec;

	float GetDT() const;
	float	dt;


private:

	void AddModule(Module* module);
	void PrepareUpdate();
	void FinishUpdate();
};
#endif __Application__