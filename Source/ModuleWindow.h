#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Dependencies/SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();
	int Width() const;
	int Height() const;

	void SetTitle(const char* title);

public:
	int width = 0;
	int height = 0;
	Uint32 flags;

	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	void		SetFullscreen(bool fullscreen);
	void		SetResizable(bool resizable);
	void		SetBorderless(bool borderless);
	void		SetFullscreenDesktop(bool fullDesktop);

};

#endif // __ModuleWindow_H__