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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init(NULL);

	// Our state
	show_demo_window = false;
	show_configuration_window = true;
	show_about_window = false;
	fullscreen = false;
	resizable = true;
	fullscreen = false;
	borderless = false;

	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	window_width = App->window->width;
	window_height = App->window->height;
	brightness = SDL_GetWindowBrightness(App->window->window);
	SDL_GetVersion(&version);



	return true;
}
update_status ModuleEditor::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return ret;
}


update_status ModuleEditor::Update(float dt)
{
	Docking();

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

			if (ImGui::MenuItem("OpenGL")) {}
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
		ImGui::Begin("About",&show_about_window);
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

	//Window configuration
	if (show_configuration_window) 
	{
			ImGui::Begin("Configuration", &show_configuration_window);
			if (ImGui::CollapsingHeader("Application"))
			{

				ImGui::SliderInt("Max FPS", &App->framerateCap, 1, 60);

				ImGui::Text("Limit Framerate:");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%d", App->framerateCap);

				sprintf_s(title, 25, "Framerate %.1f", App->fpsVec[App->fpsVec.size() - 1]);
				ImGui::PlotHistogram("##framerate", &App->fpsVec[0], App->fpsVec.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
				sprintf_s(title, 25, "Milliseconds %0.1f", App->msVec[App->msVec.size() - 1]);
				ImGui::PlotHistogram("##milliseconds", &App->msVec[0], App->msVec.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
			}
			if (ImGui::CollapsingHeader("Window"))
			{
				if (ImGui::Checkbox("Fullscreen", &fullscreen))
				{
					if (fullscreen) SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
					else SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Borderless", &borderless))
				{
					if (borderless) SDL_SetWindowBordered(App->window->window, SDL_FALSE);
					else SDL_SetWindowBordered(App->window->window, SDL_TRUE);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Full Desktop", &full_desktop))
				{
					if (full_desktop) SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
					else SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
				}
				ImGui::Separator();
				ImGui::SliderInt("Width", &window_width, 350, 1500, "%d");
				ImGui::SliderInt("Height", &window_height, 350, 1200, "%d");
				SDL_SetWindowSize(App->window->window, window_width, window_height);
				App->renderer3D->OnResize(window_width, window_height);

				ImGui::SliderFloat("Brightness", &brightness, 0, 1, "%.3f");
				SDL_SetWindowBrightness(App->window->window, brightness);

			}
			if (ImGui::CollapsingHeader("Hardware"))
			{
				ImGui::Text("SDL Version:");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%d.%d.%d", version.major, version.minor, version.patch);

				ImGui::Separator();

				ImGui::Text("CPUs: ");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%d", SDL_GetCPUCount());

				ImGui::Text("System RAM: ");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, " %d Gb", SDL_GetSystemRAM());
				ImGui::Separator();

				ImGui::Text("Caps: ");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%s", GetSystemCaps());
				ImGui::Separator();

				ImGui::Text("GPU: ");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%s", glGetString(GL_RENDERER));

				ImGui::Text("Brand: ");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%s", glGetString(GL_VENDOR));

				ImGui::Text("Version: ");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%s", glGetString(GL_VERSION));

				ImGui::Text("VRAM Budget:");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%i Mb", GetBudget());

				ImGui::Text("VRAM Usage:");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%i Mb", GetUsage());

				ImGui::Text("VRAM Available:");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%i Mb", GetAvailable());

				ImGui::Text("VRAM Reserved:");
				ImGui::SameLine();
				ImGui::TextColored(GREEN, "%i Mb", GetReserved());

			}
			if (ImGui::CollapsingHeader("OpenGL Settings")) {

				if (ImGui::Checkbox("Depth Test", &depthtest)) {
					App->renderer3D->SetDepthtest(depthtest);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Cull Face", &cullface)) {
					App->renderer3D->SetCullface(cullface);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Lightning", &lighting)) {
					App->renderer3D->SetLighting(lighting);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Color Material", &colormaterial)) {
					App->renderer3D->SetColormaterial(colormaterial);
				}

				if (ImGui::Checkbox("2D", &texture2D)) {
					App->renderer3D->SetTexture2D(texture2D);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Cube Map", &cubemap)) {
					App->renderer3D->SetCubemap(cubemap);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Polygons smooth", &polygonssmooth)) {
					App->renderer3D->SetPolygonssmooth(polygonssmooth);
				}

			}
			ImGui::End();
	}



	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	return ret;
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

void ModuleEditor::Docking()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	ImGuiViewport* viewport = ImGui::GetWindowViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;
	
	static bool open = true;
	const char* ID = "Dockspace";

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(ID, &open, window_flags);
	ImGui::PopStyleVar(3);


	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID(ID);
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	}

	ImGui::End();
}

void ModuleEditor::RequestBrowser(const char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 5);
}


const char* ModuleEditor::GetSystemCaps()
{
	Caps.clear();
	// IF the processor has certain register it will be added to the string
	if (SDL_Has3DNow())
	{
		Caps.append("3D Now, ");
	}

	if (SDL_HasAVX())
	{
		Caps.append("AVX, ");
	}

	if (SDL_HasAVX2())
	{
		Caps.append("AVX2, ");
	}

	if (SDL_HasAltiVec())
	{
		Caps.append("AltiVec, ");
	}

	if (SDL_HasMMX())
	{
		Caps.append("MMX, ");
	}

	if (SDL_HasRDTSC())
	{
		Caps.append("RDTSC, ");
	}

	if (SDL_HasSSE())
	{
		Caps.append("SSE, ");
	}

	if (SDL_HasSSE2())
	{
		Caps.append("SSE2, ");
	}

	if (SDL_HasSSE3())
	{
		Caps.append("SSE3, ");
	}

	if (SDL_HasSSE41())
	{
		Caps.append("SSE41, ");
	}

	if (SDL_HasSSE41())
	{
		Caps.append("SSE42");
	}

	return Caps.data();
}

int ModuleEditor::GetBudget() {
	int budget;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &budget);
	return budget / 1024.0f;
}

int ModuleEditor::GetUsage() {
	int usage;
	glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &usage);
	return usage / 1024.0f;
}

int ModuleEditor::GetAvailable() {
	int available;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available);
	return available / 1024.0f;
}

int ModuleEditor::GetReserved() {
	int reserved;
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &reserved);
	return reserved / 1024.0f;
}
