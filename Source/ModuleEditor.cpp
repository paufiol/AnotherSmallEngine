#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ResourceScene.h"
#include "Resource.h"
#include "ModuleResource.h"
#include "ModuleFileSystem.h"
#include "ImporterScene.h"

#include "Dependencies/ImGUI/imgui.h"
#include "Dependencies/ImGUI/imgui_internal.h"
#include "Dependencies/ImGUI/imgui_impl_sdl.h"
#include "Dependencies/ImGUI/imgui_impl_opengl3.h"

#include "Dependencies/ImGuizmo/ImGuizmo.h"
#include "PathNode.h"
#include <map>

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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImFont* font = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Medium.ttf", 15);
	//ImGui::PushFont(font);

	ImGui::StyleColorsDark(); 
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init(NULL);

	window_width = App->window->width;
	window_height = App->window->height;
	brightness = SDL_GetWindowBrightness(App->window->window);
	SDL_GetVersion(&version);
	fpsCap = App->GetFpsCap();

	//ImGui::SaveIniSettingsToDisk()

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
	
	SetupStyleFromHue(); //This is innovation, Marc
	AboutWindow();
	ConfigurationWindow();
	ConsoleWindow();
	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
	HierarchyWindow();
	InspectorWindow();
	AssetExplorerWindow();
	PlayPauseWindow();

	
	

	GUIisHovered();
	ImGui::End();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	App->scene->ImGuizmoHandling();
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
	//ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleEditor::GUIisHovered()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantCaptureMouse ? GUIhovered = true : GUIhovered = false;
	io.WantCaptureKeyboard ? isUserTyping = true : isUserTyping = false;
}


void  ModuleEditor::SetupStyleFromHue()
{
	static int hue = 140;
	static float col_main_sat = 180.f / 255.f;
	static float col_main_val = 161.f / 255.f;

	static float col_area_sat = 124.f / 255.f;
	static float col_area_val = 100.f / 255.f;

	static float col_back_sat = 59.f / 255.f;
	static float col_back_val = 40.f / 255.f;

	ImGui::Begin("Style");
	ImGui::SliderInt("master hue", &hue, 0, 255);

	float dummy;
	ImVec4 rgb;

	ImGui::ColorConvertHSVtoRGB(hue / 255.f, col_main_sat, col_main_val, rgb.x, rgb.y, rgb.z);
	ImGui::ColorEdit3("main", &rgb.x);
	ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, col_main_sat, col_main_val);

	ImGui::ColorConvertHSVtoRGB(hue / 255.f, col_area_sat, col_area_val, rgb.x, rgb.y, rgb.z);
	ImGui::ColorEdit3("area", &rgb.x);
	ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, col_area_sat, col_area_val);

	ImGui::ColorConvertHSVtoRGB(hue / 255.f, col_back_sat, col_back_val, rgb.x, rgb.y, rgb.z);
	ImGui::ColorEdit3("back", &rgb.x);
	ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, col_back_sat, col_back_val);

	ImGui::Separator();
	
	//TODO: This should be serialised; Maybe in a Loop 
	ImVec4 temp_frustum;
	
	temp_frustum.w = frustumColor.a;
	temp_frustum.x = frustumColor.r; 
	temp_frustum.y = frustumColor.g;
	temp_frustum.z = frustumColor.b; 
	ImGui::ColorEdit3("Frustum", &temp_frustum.x);
	frustumColor = Color(float(temp_frustum.x), float(temp_frustum.y), float(temp_frustum.z), float(temp_frustum.w));

	ImGui::Separator();
	
	ImVec4 temp_normal;

	temp_normal.w = NormalColor.a;
	temp_normal.x = NormalColor.r;
	temp_normal.y = NormalColor.g;
	temp_normal.z = NormalColor.b;
	ImGui::ColorEdit3("Normal", &temp_normal.x);
	NormalColor = Color(float(temp_normal.x), float(temp_normal.y), float(temp_normal.z), float(temp_normal.w));
	
	ImGui::Separator();
	

	ImVec4 temp_AABB;

	temp_AABB.w = AABBColor.a;
	temp_AABB.x = AABBColor.r;
	temp_AABB.y = AABBColor.g;
	temp_AABB.z = AABBColor.b;
	ImGui::ColorEdit3("AABB", &temp_AABB.x);
	AABBColor = Color(float(temp_AABB.x), float(temp_AABB.y), float(temp_AABB.z), float(temp_AABB.w));


	ImVec4 temp_OBB;

	temp_OBB.w = OBBColor.a;
	temp_OBB.x = OBBColor.r;
	temp_OBB.y = OBBColor.g;
	temp_OBB.z = OBBColor.b;
	ImGui::ColorEdit3("OBB", &temp_OBB.x);
	OBBColor = Color(float(temp_OBB.x), float(temp_OBB.y), float(temp_OBB.z), float(temp_OBB.w));

	
	ImGui::Separator();

	ImGui::ShowStyleEditor();
	ImGui::End();

	ImGuiStyle& style = ImGui::GetStyle();

	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor::HSV(hue / 255.f, col_main_sat, col_main_val);
	ImVec4 col_back = ImColor::HSV(hue / 255.f, col_back_sat, col_back_val);
	ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);

	style.Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(col_main.x, col_main.y, col_main.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.Colors[ImGuiCol_Tab] = ImVec4(col_area.x, col_area.y, col_area.z, 0.62f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(col_area.x, col_area.y, col_area.z, 0.92f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(col_area.x, col_area.y, col_area.z, 0.92f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(col_area.x, col_area.y, col_area.z, 0.92f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(col_area.x, col_area.y, col_area.z, 0.92f);

	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);

	style.Colors[ImGuiCol_DockingPreview] = ImVec4(col_main.x, col_main.y, col_main.z, 0.62f);

	style.Colors[ImGuiCol_NavHighlight] = ImVec4(col_text.x, col_text.y, col_text.z, 0.62f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(col_text.x, col_text.y, col_text.z, 0.62f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(col_text.x, col_text.y, col_text.z, 0.62f);
}


void ModuleEditor::PlayPauseWindow()
{
	ImGui::Begin("Play Pause");
	std::string name = App->scene->GameTime.running ? "Pause" : "Play";
	
	//std::string name = GameTimer::gameTimer ? "Stop" : "Play";

	if(ImGui::Button(name.c_str()))
	{
		//App->scene->GameTime.running = !App->scene->GameTime.running;
		
		if (App->scene->GameTime.running == false)	App->scene->GameTime.Start();
		else if (App->scene->GameTime.running == true)	App->scene->GameTime.Stop();
	}
	
	ImGui::SameLine();

	if (App->scene->GameTime.Read() > 0) {

		if (ImGui::Button("Stop"))
		{
			App->scene->GameTime.Restart();
		}
	}
	
	ImGui::SameLine();

	ImGui::Text("GameTime:");
	
	ImGui::SameLine();
	
	ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1.0f), "%.2f", (App->scene->GameTime.Read()/1000.0f));

	ImGui::End();
}

void ModuleEditor::AssetExplorerWindow()
{
	ImGui::Begin("Explorer");

	ImGui::BeginChild("Explorer Tree", ImVec2(200, 0));

	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
	ImGui::Text("WIP Asset Explorer");


	ImGui::EndChild();
	
	ImGui::SameLine();
	ImGui::BeginChild("Explorer Folder");
	
	std::map<uint32, Resource*>::iterator resourece_i = App->resources->resources.begin();
	
	/*
	*/
	for (; resourece_i != App->resources->resources.end(); resourece_i++)
	{

		ImGui::Text(resourece_i->second->assetsFile.c_str());
	
	}


	ImGui::EndChild();
	ImGui::End();
}

bool ModuleEditor::MainMenuBar()
{
	bool ret = true;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))	
		{ 
			if (ImGui::MenuItem("Save Scene"))
			{
				ResourceScene* scene = new ResourceScene();
				std::map<uint32, Resource*>::iterator it = App->resources->resources.find(App->scene->sceneUID);
				if (it != App->resources->resources.end())
				{
					scene = (ResourceScene*)it->second;
				}
				char* buffer;
				string path = SCENES_PATH;
				path.append(std::to_string(scene->UID));
				path.append(ASE_EXTENSION); 
				uint size = Importer::SceneImporter::Save(scene, &buffer);
				App->fileSystem->Save(path.c_str(), buffer, size);

			}
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
		if (ImGui::BeginMenu("Add"))
		{	
			if (ImGui::MenuItem("Create Game Object")) App->scene->CreateGameObject("New_Empty_", App->scene->root_object);
			
			if (App->scene->selected_object != nullptr)
			{
				if (ImGui::MenuItem("Create Children Object")) App->scene->CreateGameObject("New_EmptyChildren_", App->scene->selected_object);
			}

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
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Set Guizmo: Translate (W)"))	App->scene->gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
			if (ImGui::MenuItem("Set Guizmo: Rotate (E)"))		App->scene->gizmoOperation = ImGuizmo::OPERATION::ROTATE;
			if (ImGui::MenuItem("Set Guizmo: Scale (R)"))		App->scene->gizmoOperation = ImGuizmo::OPERATION::SCALE;

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
			if (ImGui::Checkbox("Draw Bounding Boxes", &App->renderer3D->drawboundingboxes)) { App->renderer3D->drawboundingboxes; }
			if (ImGui::Checkbox("Current Camera Culling", &App->camera->currentCamera->frustum_culling)) {}
		}
		if (ImGui::CollapsingHeader("Camera Settings"))
		{
			float Inspector_FOV = App->camera->currentCamera->GetFOV();
			if (ImGui::SliderFloat("FOV", &Inspector_FOV, 30, 120, "%0.2f", ImGuiSliderFlags_None)) 
			{
				App->camera->currentCamera->SetFOV(Inspector_FOV);
			}
			
			float Inspector_NearPlane = App->camera->currentCamera->GetNearPlane();
			if (ImGui::DragFloat("Near Plane", &Inspector_NearPlane))
			{ 
				App->camera->currentCamera->SetNearPlane(Inspector_NearPlane); 
			}
			
			float Inspector_FarPlane = App->camera->currentCamera->GetFarPlane();
			if (ImGui::DragFloat("Far Plane", &Inspector_FarPlane))
			{ 
				App->camera->currentCamera->SetFarPlane(Inspector_FarPlane); 
			}

			if(ImGui::Button("Change Current Camera"))
			{
				if (App->camera->currentCamera == App->camera->editorCamera) {
					App->camera->currentCamera = App->camera->gameCamera;
				}
				else if (App->camera->currentCamera == App->camera->gameCamera) {
					App->camera->currentCamera = App->camera->editorCamera;
				}
				else LOG("Something went wrong when swapping cameras");
			}
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
			if (ImGui::InputText("Name", (char*)App->scene->selected_object->name.c_str(), 64, ImGuiInputTextFlags_EnterReturnsTrue))
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
					App->scene->selected_object->components[m]->DrawInspector();
			}

			ImGui::Separator();

			const char* items[] = { "Transform", "Mesh", "Texture", "Camera" };
			static const char* current_item = NULL;
			if (ImGui::BeginCombo("-", current_item))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(items[n], is_selected))
					{
						current_item = items[n];
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)	
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			if (ImGui::Button("Add Component"))
			{
				if (strcmp(current_item, "Transform") == 0)
				{
					if (App->scene->selected_object->GetComponent(ComponentType::Transform) == nullptr)
					{
						ComponentTransform* newComponent = new ComponentTransform(App->scene->selected_object);
						App->scene->selected_object->AddComponent(newComponent);
					}
					else
					{
						LOG("ERROR: Game Objects can not have repeated components");
					}
				}
				else if (strcmp(current_item, "Mesh") == 0)
				{
					if (App->scene->selected_object->GetComponent(ComponentType::Mesh) == nullptr)
					{
						ComponentMesh* newComponent = new ComponentMesh(App->scene->selected_object);
						App->scene->selected_object->AddComponent(newComponent);
					}
					else
					{
						LOG("ERROR: Game Objects can not have repeated components");
					}
				}
				else if (strcmp(current_item, "Texture") == 0)
				{
					if (App->scene->selected_object->GetComponent(ComponentType::Material) == nullptr)
					{
						ComponentTexture* newComponent = new ComponentTexture(App->scene->selected_object);
						App->scene->selected_object->AddComponent(newComponent);
					}
					else
					{
						LOG("ERROR: Game Objects can not have repeated components");
					}
				}
				else if (strcmp(current_item, "Camera") == 0)
				{
					if (App->scene->selected_object->GetComponent(ComponentType::Camera) == nullptr)
					{
						ComponentCamera* newComponent = new ComponentCamera(App->scene->selected_object);
						App->scene->selected_object->AddComponent(newComponent);
					}
					else
					{
						LOG("ERROR: Game Objects can not have repeated components");
					}
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



		ImGui::Begin("Console", &show_console_window);

		for (int i = 0; i < log_record.size(); i++)
		{
			ImVec4 textColor = { 1.0f, 1.0f, 1.0f, 1.0f };

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
