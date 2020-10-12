#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "Dependencies/ImGui/imgui.h"
#include "Globals.h"
#include "Timer.h"
#include <vector>

class ModuleEditor : public Module 
{
public:
	ModuleEditor(bool start_enabled = true);
	~ModuleEditor();

	bool Start() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

public:
	//SDL_Window* window;
	//SDL_WindowFlags window_flags;
	ImGuiIO io;
	ImVec4 clear_color;

	std::vector<float> fps_log;
	std::vector<float> ms_log;
	Timer frame_time;
	char title[25];
	char label[32];



	bool show_demo_window;
	bool show_configuration_window;
	bool show_about_window;


	bool fullscreen;
	bool resizable;
	bool borderless;
	bool full_desktop;
	void RequestBrowser(const char*);
};

#endif //__ModuleEditor_H__