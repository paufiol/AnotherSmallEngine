#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__
#include <vector>
#include <string>
#include "Module.h"
#include "Dependencies/ImGui/imgui.h"
#include "Globals.h"
#include "Timer.h"

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

	void DrawGUI();

	void Docking();
	void RequestBrowser(const char*);
	bool MainMenuBar();
	void AboutWindow();
	void ConfigurationWindow();
	void ConsoleWindow();
	void AddLog(std::string text);
	void InspectorWindow();
	void HierarchyWindow();

	const char* GetSystemCaps();

	int GetBudget();
	int GetUsage();
	int GetAvailable();
	int GetReserved();


public:
	//SDL_Window* window;
	//SDL_WindowFlags window_flags;
	SDL_version version;
	std::string Caps;

	std::vector<std::string> log_record;
	
	char title[25];
	char label[32];
	int window_width = 0;
	int window_height = 0;
	float brightness = 0;

	bool dockingWindow = true;

	bool show_demo_window = false;
	bool show_configuration_window = true;
	bool show_about_window = false;
	bool show_console_window = true;
	bool show_hierarchy_window = true;
	bool show_inspector_window = true;

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

	bool drawNormals = false;
	bool drawWireframe = false;
	bool drawCheckerTex = false;
	bool drawTexture = true;


};

#endif //__ModuleEditor_H__