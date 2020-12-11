#ifndef __Application__
#define __Application__

#include "Globals.h"
#include "Timer.h"

class Module;												
class ModuleWindow;
class ModuleInput;
class ModuleScene;
class ModuleRenderer3D;
class ModuleEditor;
class ModuleCamera3D;
class ModuleFileSystem;
class ModuleResources;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleFileSystem* fileSystem ;
	ModuleResources* resources;

	bool debug = false;
	bool renderPrimitives = false;

	std::vector<Module*> modules;

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	std::vector<float> GetFps() { return fpsVec; }
	std::vector<float> GetMs() { return msVec; }
	int GetFpsCap() { return framerateCap; }
	void SetFpsCap(int fpsCap);
private:
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


	void AddModule(Module* module);
	void PrepareUpdate();
	void FinishUpdate();
};
#endif __Application__