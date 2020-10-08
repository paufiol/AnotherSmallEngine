#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

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

	//Set all the atributes and flags for our Gui window
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("AnotherSmallEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
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
	show_demo_window = true;
	show_window_options = false;
	show_about_window = false;
	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return true;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return true;
}

update_status ModuleEditor::Update(float dt)
{
	
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	//Top bar menu, with an option to close the editor
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("Configuration"))
		{
			if (ImGui::MenuItem("Window")) { show_window_options = !show_window_options;}
			
			if (ImGui::MenuItem("OpenGL")){}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) 
		{
			if (ImGui::MenuItem("ImGui Demo")){show_demo_window = !show_demo_window; }
			if (ImGui::MenuItem("Documentation")){ RequestBrowser("https://github.com/paufiol/AnotherSmallEngine/blob/master/README.md");}
			if (ImGui::MenuItem("Latest Release")) { RequestBrowser("https://github.com/paufiol/AnotherSmallEngine"); }
			if (ImGui::MenuItem("Report a bug")) { RequestBrowser("https://github.com/paufiol/AnotherSmallEngine/issues"); }
			if (ImGui::MenuItem("About")) { show_about_window = !show_about_window; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Close", "Alt+F4")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
		ImGui::End();
	}
	//Allocate Framerate in vecor fps_log
	if (fps_log.size() <= 100)
		fps_log.push_back(ImGui::GetIO().Framerate);
	else
		fps_log.erase(fps_log.begin());

	//Show the demo window
	if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }

	//About window
	if (show_about_window) { 
		ImGui::Begin("About"); 
		ImGui::End();
	}
	//Window configuration
	if (show_window_options) {
		ImGui::Begin("Window Options");
		if(ImGui::Checkbox("Fullscreen",&fullscreen)){}
		sprintf_s(title, 25, "Framerate %1.f", fps_log[fps_log.size()-1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0,title, 0.0f, 100.0f, ImVec2(310, 100));
		ImGui::End();
	}


	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	//Render
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
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

	SDL_GL_SwapWindow(window);
	
	return UPDATE_CONTINUE;
}

void ModuleEditor::RequestBrowser(const char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 5);
}
