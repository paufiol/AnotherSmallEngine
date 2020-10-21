#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Dependencies/ImGUI/imgui.h"
#include "Dependencies/ImGUI/imgui_internal.h"
#include "Dependencies/ImGUI/imgui_impl_sdl.h"
#include "Dependencies/ImGUI/imgui_impl_opengl3.h"

#include "Dependencies/Glew/include/GL/glew.h"
#include "Dependencies/SDL/include/SDL_opengl.h"


ModuleEditor::ModuleEditor(bool start_enabled) : Module(start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Start()
{
	LOG("Init Editor");

	//Initialize glew
	glewInit();
	frame_time.Start();

	//Set all the atributes and flags for our Gui window

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 7.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	// Our state
	show_demo_window = false;
	show_configuration_window = false;
	show_about_window = false;
	fullscreen = false;
	resizable = false;
	fullscreen = false;
	borderless = false;
	clear_color = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	window_width = App->window->width;
	window_height = App->window->height;
	brightness = SDL_GetWindowBrightness(App->window->window);
	SDL_GetVersion(&version);
	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init(0);

	return true;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyWindow(App->window->window);
	SDL_Quit();
	return true;
}

update_status ModuleEditor::Update(float dt)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();


	//Top bar menu, with an option to close the editor
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close", "Alt+F4")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Configuration")) show_configuration_window = !show_configuration_window;
			
			if (ImGui::MenuItem("OpenGL")){}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) 
		{
			if (ImGui::MenuItem("ImGui Demo"))show_demo_window = !show_demo_window; 
			if (ImGui::MenuItem("Documentation")) RequestBrowser("https://github.com/paufiol/AnotherSmallEngine/blob/master/README.md");
			if (ImGui::MenuItem("Latest Release")) RequestBrowser("https://github.com/paufiol/AnotherSmallEngine"); 
			if (ImGui::MenuItem("Report a bug")) RequestBrowser("https://github.com/paufiol/AnotherSmallEngine/issues"); 
			if (ImGui::MenuItem("About")) show_about_window = !show_about_window; 
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
		ImGui::End();
	}

	//Show the demo window
	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

	//About window
	if (show_about_window) { 
		ImGui::Begin("About",&active);
		ImGui::Text("ASE - Another Small Engine");
		ImGui::Text("Engine developed for academic purpouses.");
		ImGui::Text("By Pau Fiol & Aitor Luque");
		sprintf(label, "Github Repository (Link)");
		if (ImGui::Selectable(label, true))	RequestBrowser("https://github.com/paufiol/AnotherSmallEngine");
		ImGui::Separator();
		ImGui::Text("Libraries used:");
		ImGui::BulletText("SDL");
		ImGui::BulletText("Glew");
		ImGui::BulletText("OpenGL");
		ImGui::BulletText("ImGui");
		ImGui::BulletText("MathGeoLib");
		ImGui::BulletText("Assimp");
		ImGui::Separator();
		ImGui::Text("GNU License:"); 
		sprintf(label, "Click here to see the full License");
		if (ImGui::Selectable(label, true))	RequestBrowser("https://github.com/paufiol/AnotherSmallEngine/blob/master/LICENSE.txt");
		ImGui::End();
	}
	//Allocate Framerate in vecor fps_log
	if (fps_log.size() <= 100)
		fps_log.push_back(ImGui::GetIO().Framerate);
	else
		fps_log.erase(fps_log.begin());

	//Allocate Milliseconds in vecor ms_log
	if (ms_log.size() <= 100)
		ms_log.push_back(frame_time.Read());
	else
		ms_log.erase(ms_log.begin());

	//Window configuration
	if (show_configuration_window) {
		if (!ImGui::Begin("Configuration", &active)) 
		if (ImGui::CollapsingHeader("Application"))
		{
			sprintf_s(title, 25, "Framerate %1.f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %1.f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::Checkbox("Fullscreen", &fullscreen))
			{ 
				if (fullscreen) SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
				else SDL_SetWindowFullscreen(App->window->window,SDL_WINDOW_RESIZABLE);
			}
			if (ImGui::Checkbox("Borderless", &borderless)) 
			{ 
				if (borderless) SDL_SetWindowBordered(App->window->window, SDL_FALSE);
				else SDL_SetWindowBordered(App->window->window, SDL_TRUE);
			}
			if (ImGui::Checkbox("Full Desktop", &full_desktop))
			{
				if (full_desktop) SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				else SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
			}

			ImGui::SliderInt("Width", &window_width, 350, 1500,"%d");
			ImGui::SliderInt("Height", &window_height, 350, 1200, "%d");
			SDL_SetWindowSize(App->window->window, window_width, window_height);
			App->renderer3D->OnResize(window_width, window_height);

			ImGui::SliderFloat("Brightness", &brightness, 0, 1, "%.3f");
			SDL_SetWindowBrightness(App->window->window, brightness);

		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("SDL version %d.%d.%d", version.major, version.minor, version.patch);
			ImGui::Separator();
			ImGui::Text("CPUs: %d", SDL_GetCPUCount());
			ImGui::Text("System RAM: %dGb", SDL_GetSystemRAM());
			ImGui::Separator();
		}
		ImGui::End();
	}

	//Render
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
	return UPDATE_CONTINUE;
}

void ModuleEditor::RequestBrowser(const char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 5);
}
