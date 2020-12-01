#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"

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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	

	ImGui::StyleColorsDark(); 
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init(NULL);

	window_width = App->window->width;
	window_height = App->window->height;
	brightness = SDL_GetWindowBrightness(App->window->window);
	SDL_GetVersion(&version);
	fpsCap = App->GetFpsCap();


	return true;
}
update_status ModuleEditor::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;



	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return ret;
}


update_status ModuleEditor::Update(float dt)
{

	Docking();
	if (!MainMenuBar()) return UPDATE_STOP;
	AboutWindow();
	ConfigurationWindow();
	ConsoleWindow();
	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
	HierarchyWindow();
	InspectorWindow();
	GUIisHovered();
	ImGui::End();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	log_record.clear();
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();
	return true;
}

void ModuleEditor::DrawGUI()
{
	
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleEditor::GUIisHovered()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantCaptureMouse ? GUIhovered = true : GUIhovered = false;
}

bool ModuleEditor::MainMenuBar()
{
	bool ret = true;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))	
		{ 
			if(ImGui::MenuItem("Exit")) ret = false;
			
			ImGui::EndMenu(); 
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration")) show_configuration_window = !show_configuration_window;
			if (ImGui::MenuItem("Console")) show_console_window = !show_console_window;
			if (ImGui::MenuItem("Hierarchy"))show_hierarchy_window = !show_hierarchy_window;
			if (ImGui::MenuItem("Inspector"))show_inspector_window = !show_inspector_window;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Add Primitive"))
		{
			//if (ImGui::MenuItem("Cube")) Importer::MeshImporter::Import("Assets/Primitives/Cube.FBX");
			//if (ImGui::MenuItem("Sphere"))Importer::MeshImporter::Import("Assets/Primitives/Sphere.FBX");	
			//if (ImGui::MenuItem("Cylinder")) Importer::MeshImporter::Import("Assets/Primitives/Cylinder.FBX");
			//if (ImGui::MenuItem("Cone")) Importer::MeshImporter::Import("Assets/Primitives/Cone.FBX");
			//if (ImGui::MenuItem("Pyramid")) Importer::MeshImporter::Import("Assets/Primitives/Pyramid.FBX");
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
	return ret;
}
void ModuleEditor::AboutWindow()
{
	if (show_about_window) {
		ImGui::Begin("About", &show_about_window);
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
}

void ModuleEditor::ConfigurationWindow()
{
	if (show_configuration_window)
	{
		ImGui::Begin("Configuration", &show_configuration_window);
		if (ImGui::CollapsingHeader("Application"))
		{

			ImGui::SliderInt("Max FPS", &fpsCap, 1, 60);
			App->SetFpsCap(fpsCap);
			ImGui::Text("Limit Framerate:");
			ImGui::SameLine();
			ImGui::TextColored(GREEN, "%d", fpsCap);

			sprintf_s(title, 25, "Framerate %.1f", App->GetFps()[App->GetFps().size() - 1]);
			ImGui::PlotHistogram("##framerate", &App->GetFps()[0], App->GetFps().size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", App->GetMs()[App->GetMs().size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &App->GetMs()[0], App->GetMs().size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::Checkbox("Fullscreen", &fullscreen)) App->window->SetFullscreen(fullscreen);
			if (ImGui::Checkbox("Borderless", &borderless)) App->window->SetBorderless(borderless);
			if (ImGui::Checkbox("Full Desktop", &full_desktop)) App->window->SetFullscreenDesktop(full_desktop);

			ImGui::Separator();
			if(ImGui::SliderInt("Width", &window_width, 350, 1500, "%d")) SDL_SetWindowSize(App->window->window, window_width, window_height);
			if(ImGui::SliderInt("Height", &window_height, 350, 1200, "%d")) SDL_SetWindowSize(App->window->window, window_width, window_height);
			

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
			if (ImGui::Checkbox("Cull Face", &cullface)) {
				App->renderer3D->SetCullface(cullface);
			}
			if (ImGui::Checkbox("Lightning", &lighting)) {
				App->renderer3D->SetLighting(lighting);
			}
			if (ImGui::Checkbox("2D", &texture2D)) {
				App->renderer3D->SetTexture2D(texture2D);
			}
			if (ImGui::Checkbox("Color Material", &colormaterial)) {
				App->renderer3D->SetColormaterial(colormaterial);
			}
			if (ImGui::Checkbox("Cube Map", &cubemap)) {
				App->renderer3D->SetCubemap(cubemap);
			}
			if (ImGui::Checkbox("Polygons smooth", &polygonssmooth)) {
				App->renderer3D->SetPolygonssmooth(polygonssmooth);
			}

		}
		if (ImGui::CollapsingHeader("Draw Settings"))
		{
			if (ImGui::Checkbox ("Draw Normals", &drawNormals)){}
			if (ImGui::Checkbox ("Wireframe Mode", &drawWireframe)) {}
			if (ImGui::Checkbox("Enable Checker Tex", &drawCheckerTex)) { drawTexture = false; }
			if (ImGui::Checkbox("Enable Texture", &drawTexture)) { drawCheckerTex = false; }
		}

		ImGui::End();
	}
}

void ModuleEditor::InspectorWindow()
{
	if (show_inspector_window)
	{
		ImGui::Begin("Inspector", &show_inspector_window);

		if(App->scene->selected_object != nullptr)
		{
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.33f);
			char tempName[64];
			strcpy_s(tempName, App->scene->selected_object->name.c_str());
			if (ImGui::InputText("Name", tempName, IM_ARRAYSIZE(tempName), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				App->scene->selected_object->SetName(tempName);
			}
			ImGui::Separator();

			if (ImGui::Checkbox("Object Enabled", &enableObject))
			{
				enableObject ? App->scene->selected_object->Enable() : App->scene->selected_object->Disable();
			}

			ImGui::Text("Parent: ");
			ImGui::SameLine();
			if (App->scene->selected_object->parent != nullptr)
			{
				ImGui::TextColored(GREEN, "%s", App->scene->selected_object->parent->name.c_str());
			}
			else ImGui::Text("No Parent");


			if (ImGui::Button("Delete Object"))
			{
				ImGui::OpenPopup("Delete Object", ImGuiPopupFlags_None);
			}
			if (ImGui::BeginPopupModal("Delete Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("This action cannot be undone!\n Are you sure? \n\n");
				ImGui::Separator();

				if (ImGui::Button("Delete", ImVec2(120, 0))) 
				{ 
					App->scene->DeleteGameObject(App->scene->selected_object);
					ImGui::CloseCurrentPopup(); 
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}

			ImGui::Separator();

			for (uint m = 0; m < App->scene->selected_object->components.size(); m++)
			{
				if (App->scene->selected_object->selected)
				{
					App->scene->selected_object->components[m]->DrawInspector();
				}

			}	
		}
		ImGui::End();
	}
}

void ModuleEditor::DrawHierarchyLevel(GameObject* rootObject)
{
	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
	if(rootObject->children.empty()) treeFlags |= ImGuiTreeNodeFlags_Leaf ;
	if (rootObject == App->scene->selected_object) treeFlags |= ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_Framed;
	ImGui::AlignTextToFramePadding();

	if (ImGui::TreeNodeEx(rootObject->name.c_str(), treeFlags))
	{
		if (ImGui::IsItemClicked())						// To select Scene or the House needs to be opened
		{
			App->scene->SelectObject(rootObject);
			rootObject->selected = true;
		}
		if (rootObject != App->scene->root_object)
		{	
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("Dragged_Object", rootObject, sizeof(GameObject));
				childObject = rootObject;
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged_Object"))
				{
					rootObject->AddChildren(childObject);

					childObject = nullptr;
				}
				
				ImGui::EndDragDropTarget();
			}
		}
			if (!rootObject->children.empty())
			{
				for (uint i = 0; i < rootObject->children.size(); ++i)
				{
					DrawHierarchyLevel(rootObject->children[i]);
				}
			}

		ImGui::TreePop();	
	}
};

void ModuleEditor::HierarchyWindow()
{
	if (show_hierarchy_window)
	{
		ImGui::Begin("Hierarchy", &show_hierarchy_window);
		
		DrawHierarchyLevel(App->scene->root_object);


		ImGui::End();
	}
}


void ModuleEditor::ConsoleWindow()
{
	if (show_console_window) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 6));

		ImVec4 textColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		ImGui::Begin("Console", &show_console_window);

		for (int i = 0; i < log_record.size(); i++)
		{
			if (strstr(log_record[i].c_str(), "ERROR") != nullptr)
			{
				textColor = { 1.0f, 0.0f, 0.3f, 0.7f };
			}

			ImGui::PushStyleColor(ImGuiCol_Text, textColor);

			ImGui::Text("%s", log_record[i].c_str());

			ImGui::PopStyleColor();
		}

		ImGui::PopStyleVar();

		if (scrollDown) ImGui::SetScrollHere(1.0f);
		scrollDown = false;

		ImGui::End();
	}
}

void ModuleEditor::AddLog(string text) {

	if (&log_record != NULL) {
		log_record.push_back(text);
		scrollDown = true;
	}
}

void ModuleEditor::Docking()
{
	ImGuiWindowFlags window = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetWindowViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Docking", &dockingWindow, window);
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	ImGuiID dockspace_id = ImGui::GetID("Dockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
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
