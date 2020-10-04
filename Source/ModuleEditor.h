#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "Globals.h"

class ModuleEditor : public Module 
{
public:
	ModuleEditor(bool start_enabled = true);
	~ModuleEditor();

	bool Start() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

public:
	SDL_Window* window;
	SDL_WindowFlags window_flags;
	SDL_GLContext gl_context;
	bool show_demo_window;
	bool show_another_window;



	bool done = false;
};

#endif //__ModuleEditor_H__