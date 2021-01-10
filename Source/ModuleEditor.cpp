#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "ResourceScene.h"
#include "ResourceMaterial.h"
#include "ResourceShader.h"
#include "Resource.h"
#include "ModuleResource.h"
#include "ModuleFileSystem.h"
#include "JsonConfig.h"

#include "ImporterScene.h"
#include "ImporterMaterials.h"
#include "ImporterShader.h"

#include "Dependencies/ImGUI/imgui.h"
#include "Dependencies/ImGUI/imgui_internal.h"
#include "Dependencies/ImGUI/imgui_impl_sdl.h"
#include "Dependencies/ImGUI/imgui_impl_opengl3.h"
#include "Dependencies/Devil/Include/ilut.h"
#include "Dependencies/ImGuizmo/ImGuizmo.h"

#include "PathNode.h"
#include <map>

#include "Dependencies/Glew/include/GL/glew.h"
#include "Dependencies/SDL/include/SDL_opengl.h"

#include <fstream>




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

	//Load the icons into TextureResource
	LoadIcons();
	UpdateAssetExplorer();
	currentFolder = assetsFolder;

	//ImGui::SaveIniSettingsToDisk()

	editor.SetPalette(TextEditor::GetDarkPalette());

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
	DropTargetWindow();
	PlayPauseWindow();
	TextEditorWindow();
	
	

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

void ModuleEditor::UpdateAssetExplorer()
{
	std::vector<std::string> ignore_ext;
	ignore_ext.push_back("meta");
	assetsFolder = App->fileSystem->GetAllFiles("Assets", nullptr, &ignore_ext);

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
	
	ImVec4 temp_normal;
	temp_normal.w = NormalColor.a;
	temp_normal.x = NormalColor.r;
	temp_normal.y = NormalColor.g;
	temp_normal.z = NormalColor.b;
	ImGui::ColorEdit3("Normal", &temp_normal.x);
	NormalColor = Color(float(temp_normal.x), float(temp_normal.y), float(temp_normal.z), float(temp_normal.w));
	
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

	ExplorerIconsTint = ImVec4(col_main.x + 0.2f, col_main.y + 0.2f, col_main.z + 0.2f, 1.0f);

	style.Colors[ImGuiCol_Text] =					ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] =			ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] =				ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
	style.Colors[ImGuiCol_Border] =					ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] =			ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] =				ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] =			ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] =			ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] =				ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] =		ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] =			ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] =				ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] =			ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] =			ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] =	ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] =	ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] =				ImVec4(col_main.x, col_main.y, col_main.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] =				ImVec4(col_main.x, col_main.y, col_main.z, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] =		ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Button] =					ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] =			ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] =			ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Header] =					ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] =			ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] =			ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] =				ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] =		ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] =		ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] =				ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] =		ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] =			ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] =	ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] =			ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
	style.Colors[ImGuiCol_DragDropTarget] =			ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] =	ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	style.Colors[ImGuiCol_Tab] =					ImVec4(col_area.x, col_area.y, col_area.z, 0.62f);
	style.Colors[ImGuiCol_TabActive] =				ImVec4(col_area.x, col_area.y, col_area.z, 0.92f);
	style.Colors[ImGuiCol_TabHovered] =				ImVec4(col_area.x, col_area.y, col_area.z, 0.92f);
	style.Colors[ImGuiCol_TabUnfocused] =			ImVec4(col_area.x, col_area.y, col_area.z, 0.92f);
	style.Colors[ImGuiCol_TabUnfocusedActive] =		ImVec4(col_area.x, col_area.y, col_area.z, 0.92f);
	style.Colors[ImGuiCol_SeparatorHovered] =		ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_SeparatorActive] =		ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_DockingPreview] =			ImVec4(col_main.x, col_main.y, col_main.z, 0.62f);
	style.Colors[ImGuiCol_NavHighlight] =			ImVec4(col_text.x, col_text.y, col_text.z, 0.62f);
	style.Colors[ImGuiCol_NavWindowingHighlight] =	ImVec4(col_text.x, col_text.y, col_text.z, 0.62f);
	style.Colors[ImGuiCol_NavWindowingDimBg] =		ImVec4(col_text.x, col_text.y, col_text.z, 0.62f);
}


void ModuleEditor::PlayPauseWindow()
{
	ImGui::Begin("Play Pause");
	std::string name = App->scene->GameTime.running ? "Pause" : "Play";
	
	//std::string name = GameTimer::gameTimer ? "Stop" : "Play";

	if(ImGui::Button(name.c_str()))
	{
		//App->scene->GameTime.running = !App->scene->GameTime.running;
		
		if (App->scene->GameTime.running == false)
		{
			scene = App->resources->CreateNewResource("", ResourceType::Scene, "tempScene");
			scene->libraryFile = SCENES_PATH;
			scene->libraryFile.append(scene->name.c_str());
			scene->libraryFile.append(".scene");

			scene->assetsFile = SCENES_FOLDER;
			scene->assetsFile.append(scene->name.c_str());
			scene->assetsFile.append(".scene");

			App->resources->SaveResource(scene);
			
			App->scene->GameTime.Resume();
		}
		else if (App->scene->GameTime.running == true)
		{
			App->scene->GameTime.Pause();
		}
	}

	
	ImGui::SameLine();

	if (App->scene->GameTime.Read() > 0) {

		if (ImGui::Button("Stop"))
		{
			std::map<uint32, Resource*>::iterator it = App->resources->importedResources.begin();
			for (; it != App->resources->importedResources.end(); it++)
			{
				if (it->second->type == ResourceType::Scene && it->second->name == "tempScene" && it->second->libraryFile == "Library/Scenes/tempScene.scene")
				{
					App->resources->LoadResource(it->second->UID);

					std::string sceneToDelete = it->second->assetsFile;
					App->fileSystem->Remove(sceneToDelete.c_str());

					sceneToDelete = SCENES_PATH;
					sceneToDelete.append(scene->name.c_str());
					sceneToDelete.append(".scene");

					App->fileSystem->Remove(sceneToDelete.c_str());
				}
			}
			
			App->scene->GameTime.Restart();
		}
	}
	
	ImGui::SameLine(100);

	ImGui::Text("GameTime:");
	
	ImGui::SameLine();
	
	ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1.0f), "%.2f", (App->scene->GameTime.Read()/1000.0f));

	ImGui::End();
}

void ModuleEditor::LoadIcons()
{
	modelIcon = new ResourceTexture();
	defaultIcon = new ResourceTexture();
	folderIcon = new ResourceTexture();
	returnIcon = new ResourceTexture();

	char* buffer = nullptr;
	uint size = App->fileSystem->Load("Assets/Icons/FBX_Icon_x4.png", &buffer);
	if (size > 0) Importer::TextureImporter::ImportTexture(modelIcon, buffer, size);
	RELEASE_ARRAY(buffer);

	size = 0;
	size = App->fileSystem->Load("Assets/Icons/ASE_Icon_x4.png", &buffer);
	if (size > 0) Importer::TextureImporter::ImportTexture(defaultIcon, buffer, size);
	RELEASE_ARRAY(buffer);

	size = 0;
	size = App->fileSystem->Load("Assets/Icons/Folder_Icon_x4.png", &buffer);
	if (size > 0) Importer::TextureImporter::ImportTexture(folderIcon, buffer, size);
	RELEASE_ARRAY(buffer);

	size = 0;
	size = App->fileSystem->Load("Assets/Icons/Return_Icon_x1.png", &buffer);
	if (size > 0) Importer::TextureImporter::ImportTexture(returnIcon, buffer, size);
	RELEASE_ARRAY(buffer);
}

void ModuleEditor::AssetExplorerWindow()
{
	Timer timer;
	timer.Start();
	if (updateTimer.ReadSec() > updateTime)
	{
		UpdateAssetExplorer();
		updateTimer.Start();
	}
	double time = timer.Read();
	
	ImGui::Begin("Assets Tree");
	AssetsTree(assetsFolder);
	ImGui::End();
	ImGui::Begin("Assets Explorer");
	AssetsExplorer(currentFolder);
	ImGui::End();
}

void ModuleEditor::AssetsTree(PathNode& assetFolder)
{
	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen 
		| ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (assetFolder.IsLastFolder()) treeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (currentFolder == assetFolder) treeFlags |= ImGuiTreeNodeFlags_Selected;

	if (!assetFolder.isFile) //This way we only show folders in the Tree
	{
		if (ImGui::TreeNodeEx(assetFolder.localPath.c_str(), treeFlags, assetFolder.localPath.c_str()))
		{
			if (ImGui::IsItemClicked()) 
			{
				nextFolder = previousFolder;
				currentFolder = assetFolder; 
			}
			if (!assetFolder.IsLastFolder())
			{
				for (uint i = 0; i < assetFolder.children.size(); i++)
				{
					AssetsTree(assetFolder.children[i]);
				}
				ImGui::TreePop();
			}
		}
	}
}

void ModuleEditor::AssetsExplorer(PathNode& assetFolder)
{
	//if(assetFolder.path == "Assets") previousFolder = assetFolder;

	nextFolder = previousFolder;

	uint row = 0;
	uint offset = 50;
	uint column = (float)(ImGui::GetWindowWidth() / (float)(iconSize + offset)); // Window.w / Item.w
	ImVec2 cursor = ImGui::GetCursorPos();
	ImVec2 flipV = ImVec2(0.0f, 1.0f);
	ImVec2 flipH = ImVec2(1.0f, 0.0f);

	ImGui::Text(assetFolder.localPath.c_str());

	ImGui::Separator();

	ImGui::BeginChild(1);

	
	for(uint i = 0; i < assetFolder.children.size(); i++)
	{
		ImGui::PushID(i);

		//Get Id from meta file and the Resource from that ID
		std::string meta = assetFolder.children[i].path + ".meta";
		char* buffer = nullptr;
		uint size = App->fileSystem->Load(meta.c_str(), &buffer);
		uint32 UID = 0;
		

		ImGui::SetCursorPosX((i - (row * column)) * (iconSize + offset) + offset);
		ImGui::SetCursorPosY(row * (iconSize + offset));

		if (size > 0)
		{
			UID = JsonConfig(buffer).GetNumber("UID");
			resource = App->resources->GetResourceInMemory(UID);

			textureIcon = (ResourceTexture*)resource;

			if (textureIcon->id > MAX_TEXTURES) textureIconLoaded = false;

			if(resource->type == ResourceType::Texture && !textureIconLoaded) App->resources->LoadResource(UID);
			
			

			switch (resource->type)
			{
			case ResourceType::Model:
				ImGui::ImageButton((ImTextureID)modelIcon->id, ImVec2(iconSize, iconSize), flipV, flipH, - 1, ImVec4(0, 0, 0, 0), ExplorerIconsTint);
				break;
			case ResourceType::Texture:
				ImGui::ImageButton((ImTextureID)textureIcon->id, ImVec2(iconSize, iconSize), flipV, flipH);

				break;
			case ResourceType::Folder:
				ImGui::ImageButton((ImTextureID)folderIcon->id, ImVec2(iconSize, iconSize), flipV, flipH, -1, ImVec4(0, 0, 0, 0), ExplorerIconsTint);

				break;
			default:
				ImGui::ImageButton((ImTextureID)defaultIcon->id, ImVec2(iconSize, iconSize), flipV, flipH, -1, ImVec4(0, 0, 0, 0), ExplorerIconsTint);

				break;
			}

			if (ImGui::IsItemHovered() && resource->type != ResourceType::Folder)
			{
				Hovered_UID = JsonConfig(buffer).GetNumber("UID");
			}
			RELEASE_ARRAY(buffer);
		}
		else
		{
			ImGui::ImageButton((ImTextureID)folderIcon->id, ImVec2(iconSize, iconSize), flipV, flipH, -1, ImVec4(0, 0, 0, 0), ExplorerIconsTint);
		}



	

		if (ImGui::IsItemClicked() && !assetFolder.children[i].isFile)
		{
			nextFolder = assetFolder.children[i];
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) ImGui::OpenPopup("ImportPopUp");

		if (ImGui::BeginPopup("ImportPopUp"))
		{
			ImGui::IsItemHovered();
			if (ImGui::Selectable("Import Asset"))
			{
				App->resources->LoadResource(Hovered_UID);
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginDragDropSource())
		{
			show_dropTarget_window = true;
			ImGui::SetDragDropPayload("Asset", &UID, sizeof(uint32));

			switch (resource->type)
			{
			case ResourceType::Model:
				ImGui::Image((ImTextureID)modelIcon->id, ImVec2(iconSize, iconSize), flipV, flipH);
				break;
			case ResourceType::Texture:
				ImGui::Image((ImTextureID)textureIcon->id, ImVec2(iconSize, iconSize), flipV, flipH);
				break;
			case ResourceType::Shader:
				ImGui::Image((ImTextureID)folderIcon->id, ImVec2(iconSize, iconSize), flipV, flipH);
				break;
			}
			ImGui::EndDragDropSource();
		}

		ImGui::SetCursorPosX((i - (row * column)) * (iconSize + offset) + offset);
		ImGui::SetCursorPosY(row * (iconSize + offset) + iconSize + offset/5);

		ImGui::Text(assetFolder.children[i].localPath.c_str());

		if ((i + 1) % column == 0) row++;

		ImGui::PopID();
	}

	ImGui::EndChild();

	if (nextFolder.path != "") assetFolder = nextFolder;
	else textureIconLoaded = true;
}

void ModuleEditor::DropTargetWindow()
{
	if (show_dropTarget_window)
	{
		ImGui::SetNextWindowSize({ 500, 400 });
		ImGui::SetNextWindowPos({ 300, 50 });

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;

		ImGui::Begin("DropTarget", &show_dropTarget_window, flags);
		
		//ImGui::Rect
		//ImGui::
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2);
		ImGui::Text("Drop asset here:");

		ImGui::SetCursorPosX(0);
		ImGui::SetCursorPosY(0);
		ImGui::InvisibleButton("Drop asset here:", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
		
		if (ImGui::BeginDragDropTarget())
		{
			ResourceMaterial* material = new ResourceMaterial();
			ComponentMaterial* compMaterial;
		
			//ImGuiDragDropFlags_
				
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset", ImGuiDragDropFlags_AcceptBeforeDelivery))
			{
				uint32 UID = *(const uint32*)payload->Data;
				Resource* resource = App->resources->GetResourceInMemory(UID);

				switch (resource->type)
				{
				case ResourceType::Model:
					App->resources->LoadResource(UID);
					break;
				case ResourceType::Texture:

					compMaterial = (ComponentMaterial*)App->scene->selected_object->GetComponent(ComponentType::Material);
					compMaterial->GetMaterial()->SetTexture((ResourceTexture*)App->resources->LoadResource(UID));
					break;
				case ResourceType::Shader:

					if ((ComponentMaterial*)App->scene->selected_object->GetComponent(ComponentType::Material) == nullptr)
					{
						break;
					}

					compMaterial = (ComponentMaterial*)App->scene->selected_object->GetComponent(ComponentType::Material);
					compMaterial->GetMaterial()->SetShader((ResourceShader*)App->resources->LoadResource(UID));

					break;
				}
				// Else make a pop up Error
			}
			ImGui::EndDragDropTarget();
			show_dropTarget_window = false;
		}
		ImGui::End();
	}
}

void ModuleEditor::TextEditorWindow()
{

	if (show_texteditor_window) 
	{
		if (show_saveeditor_popup)
		{
			ImGui::OpenPopup("Save Previous File"); 
			show_saveeditor_popup = false;
		}

		if (ImGui::BeginPopupModal("Save Previous File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Do you want to save changes before closing the editor? \n\n");
			if (ImGui::Button("Save Changes"))
			{
				std::string textToSave = editor.GetText();
				App->fileSystem->Remove(fileToEdit.c_str());
				App->fileSystem->Save(fileToEdit.c_str(), textToSave.c_str(), editor.GetText().size());

				glDetachShader(shaderToRecompile->shaderProgramID, shaderToRecompile->vertexID);
				glDetachShader(shaderToRecompile->shaderProgramID, shaderToRecompile->fragmentID);
				glDeleteProgram(shaderToRecompile->shaderProgramID);

				Importer::ShaderImporter::Import(shaderToRecompile->assetsFile.c_str(), shaderToRecompile);

				show_saveeditor_popup = false;
				show_texteditor_window = true;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Don't Save"))
			{
				show_saveeditor_popup = false;
				show_texteditor_window = true;

				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (ImGui::Begin("Text Editor", &show_texteditor_window, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse))
		{

			//Update
			auto cpos = editor.GetCursorPosition();
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save"))
					{
						std::string textToSave = editor.GetText();

						App->fileSystem->Remove(fileToEdit.c_str());
						App->fileSystem->Save(fileToEdit.c_str(), textToSave.c_str(), editor.GetText().size());
					
						glDetachShader(shaderToRecompile->shaderProgramID, shaderToRecompile->vertexID);
						glDetachShader(shaderToRecompile->shaderProgramID, shaderToRecompile->fragmentID);
						glDeleteProgram(shaderToRecompile->shaderProgramID);

						Importer::ShaderImporter::Import(shaderToRecompile->assetsFile.c_str(), shaderToRecompile);
					}
			
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					bool ro = editor.IsReadOnly();
					if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
						editor.SetReadOnly(ro);
					ImGui::Separator();

					if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
						editor.Undo();
					if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
						editor.Redo();

					ImGui::Separator();

					if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
						editor.Copy();
					if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
						editor.Cut();
					if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
						editor.Delete();
					if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
						editor.Paste();

					ImGui::Separator();

					if (ImGui::MenuItem("Select all", nullptr, nullptr))
						editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("View")) //Might not be necessary? 
				{
					if (ImGui::MenuItem("Dark palette"))
						editor.SetPalette(TextEditor::GetDarkPalette());
					if (ImGui::MenuItem("Light palette"))
						editor.SetPalette(TextEditor::GetLightPalette());
					if (ImGui::MenuItem("Retro blue palette"))
						editor.SetPalette(TextEditor::GetRetroBluePalette());
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
				editor.IsOverwrite() ? "Ovr" : "Ins",
				editor.CanUndo() ? "*" : " ",
				editor.GetLanguageDefinition().mName.c_str(), fileToEdit.c_str());

			editor.Render("TextEditor");
			ImGui::End();
		}
	}
}

void ModuleEditor::CallTextEditor(ResourceMaterial* resource)
{
	//Only Handles GLSL
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::GLSL();

	fileToEdit = resource->GetShader()->assetsFile;
	editor.SetShowWhitespaces(false);

	std::ifstream t(fileToEdit.c_str());
	if (t.good())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		editor.SetText(str);
	}

	show_saveeditor_popup = true;
	show_texteditor_window = true;

	shaderToRecompile = resource->GetShader();
}

bool ModuleEditor::MainMenuBar()
{
	bool ret = true;
	std::string menuAction = "";
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))	
		{ 
			if (ImGui::MenuItem("Save Scene"))
			{
				scene = App->resources->CreateNewResource("", ResourceType::Scene, "scene");
				menuAction = "Save Scene";
			}
			if (ImGui::MenuItem("Load Scene"))
			{
				
				std::map<uint32, Resource*>::iterator it = App->resources->importedResources.begin();
				for (; it != App->resources->importedResources.end(); it++)
				{
					if (it->second->type == ResourceType::Scene)
					{
						scenesInMemory.push_back((ResourceScene*)it->second);
					}
				}

				if(!scenesInMemory.empty()) selectedScene = scenesInMemory.front()->name;

				menuAction = "Load Scene";
			}
			if(ImGui::MenuItem("Exit")) ret = false;

			ImGui::EndMenu(); 
		}
		
		//Set Next window Pos and Size
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter() , ImGuiCond_Appearing, ImVec2(0.5, 0.5));
		ImGui::SetNextWindowSize(ImVec2(250.0f, 150.0f));

		//Set next Pop based on a string
		if (menuAction == "Save Scene") ImGui::OpenPopup("Save Scene Options");
		

		if (ImGui::BeginPopupModal("Save Scene Options", NULL, ImGuiWindowFlags_NoResize))
		{
			
			//ImGui::Spacing();
			//ImGui::TextColored(sceneTextColor, scene->libraryFile.c_str());
			ImGui::Spacing();
			
			if (ImGui::InputText("Scene Name:", (char*)sceneName.c_str(), 64, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				scene->libraryFile = SCENES_PATH ;
				scene->libraryFile.append(sceneName.c_str());
				scene->libraryFile.append(".scene");

				scene->assetsFile = SCENES_FOLDER;
				scene->assetsFile.append(sceneName.c_str());

				scene->name = sceneName.c_str();

				//sceneTextColor = ImVec4(0, 0.8, 0, 1);

				App->resources->SaveResource(scene);

				ImGui::CloseCurrentPopup();
				//allowSaveOrLoad = true;

			}


			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.6, 0.6, 0.6, 1), "Press 'Enter' to save the scene.");
			ImGui::Spacing();

			//ImGui::Spacing();
			//if (allowSaveOrLoad)
			//{
			//	if (ImGui::Button("Save"))
			//	{
			//		App->resources->SaveResource(scene);
			//		allowSaveOrLoad = false;
			//		ImGui::CloseCurrentPopup();
			//	}
			//}


			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}


			
			ImGui::EndPopup();
		}

		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5, 0.5));
		ImGui::SetNextWindowSize(ImVec2(250.0f, 150.0f));

		if (menuAction == "Load Scene") ImGui::OpenPopup("Load Scene Options");

		if (ImGui::BeginPopupModal("Load Scene Options", NULL, ImGuiWindowFlags_NoResize))
		{
			ImGui::Text("Choose the secne to load");
			ImGui::Spacing();
			
			if (ImGui::BeginCombo("Scenes", selectedScene.c_str(), ImGuiComboFlags_PopupAlignLeft))
			{
				for (uint i = 0; i < scenesInMemory.size(); i++)
				{
					const bool is_selected = (scenesInMemory[i]->name == selectedScene);
					if (ImGui::Selectable(scenesInMemory[i]->name.c_str(), is_selected))
					{	
						sceneToLoad = scenesInMemory[i];
						selectedScene = scenesInMemory[i]->name;
						allowSaveOrLoad = true;
					}

				}
				
				ImGui::EndCombo();
			}

			ImGui::Spacing();

			if (allowSaveOrLoad)
			{
				if (ImGui::Button("Load"))
				{
					if (sceneToLoad) App->resources->LoadResource(sceneToLoad->UID);
					sceneToLoad = nullptr;
					scenesInMemory.clear();
					allowSaveOrLoad = false;
					ImGui::CloseCurrentPopup();
				}
			}


			ImGui::Spacing();

			if (ImGui::Button("Cancel"))
			{
				scenesInMemory.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();

			




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
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Set Guizmo: Translate (W)","W"))	App->scene->gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
			if (ImGui::MenuItem("Set Guizmo: Rotate (E)","E"))		App->scene->gizmoOperation = ImGuizmo::OPERATION::ROTATE;
			if (ImGui::MenuItem("Set Guizmo: Scale (R)","R"))		App->scene->gizmoOperation = ImGuizmo::OPERATION::SCALE;

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
			if (ImGui::Checkbox("Texture2D", &texture2D)) {
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
			if (App->scene->selected_object != nullptr) {
				for (uint m = 0; m < App->scene->selected_object->components.size(); m++)
				{
						App->scene->selected_object->components[m]->DrawInspector();
				}
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
						ComponentMaterial* newComponent = new ComponentMaterial(App->scene->selected_object);
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
