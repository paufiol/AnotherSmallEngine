#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "Dependencies/ImGui/imgui.h"
#include "Globals.h"
#include "Timer.h"

#include <vector>

#define GREEN ImVec4(0.0f, 1.0f, 0.0f, 1.0f)

class ModuleEditor : public Module 
{
public:
	ModuleEditor(bool start_enabled = true);
	~ModuleEditor();

	bool Start() override;
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp() override;

	void Docking();

	const char* GetSystemCaps();


	int GetBudget();
	int GetUsage();
	int GetAvailable();
	int GetReserved();
	std::string Caps;

public:
	//SDL_Window* window;
	//SDL_WindowFlags window_flags;
	ImGuiIO io;
	ImVec4 clear_color;
	SDL_version version;
	
	char title[25];
	char label[32];
	int window_width;
	int window_height;
	float brightness;

	bool dockingWindow = true;

	bool show_demo_window = false;
	bool show_configuration_window = false;
	bool show_about_window = false;

	bool depthtest = false;
	bool cullface = false;
	bool lighting = false;
	bool colormaterial = false;
	bool texture2D = false;
	bool cubemap = false;
	bool polygonssmooth = false;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool full_desktop = false;
	void RequestBrowser(const char*);
};

#endif //__ModuleEditor_H__