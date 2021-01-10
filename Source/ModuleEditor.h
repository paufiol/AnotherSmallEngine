#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__
#include "Module.h"
#include "Dependencies/ImGui/imgui.h"
#include "ResourceTexture.h"
#include "PathNode.h"
#include "Globals.h"
#include "Timer.h"
#include "Color.h"

#include "Dependencies/ImGui/ColorTextEditor/TextEditor.h"

using namespace std;

class GameObject;
class Resource;
class ResourceTexture;
class ResourceShader;
class ResourceMaterial;
class ResourceScene;

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
	void UpdateAssetExplorer();

	void Docking();
	void RequestBrowser(const char*);
	bool MainMenuBar();
	void AboutWindow();
	void ConfigurationWindow();
	void ConsoleWindow();
	void AddLog(string text);
	void InspectorWindow();
	void HierarchyWindow();
	void DrawHierarchyLevel(GameObject* rootObject);
	void SetupStyleFromHue();
	void PlayPauseWindow();
	void LoadIcons();
	void AssetExplorerWindow();
	void AssetsTree(PathNode& assetFolder);
	void AssetsExplorer(PathNode& assetFolder);
	void DropTargetWindow();
	void TextEditorWindow();

	void CallTextEditor(ResourceMaterial* resource);

	void GUIisHovered();

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
	GameObject* childObject = nullptr;

	PathNode assetsFolder;
	PathNode currentFolder;

	uint updateTime = 5;
	Timer updateTimer;

	ResourceTexture* modelIcon = nullptr;
	ResourceTexture* defaultIcon = nullptr;
	ResourceTexture* folderIcon = nullptr;
	ResourceTexture* returnIcon = nullptr;

	ResourceShader* shaderToRecompile = nullptr;

	Resource* scene = nullptr;
	ResourceScene* sceneToLoad = nullptr;
	std::vector<ResourceScene*> scenesInMemory;
	std::string sceneName;
	std::string selectedScene;
	ImVec4 sceneTextColor = ImVec4(0.8, 0, 0, 1);

	PathNode nextFolder;
	PathNode previousFolder;
	uint32 Hovered_UID = 0;

	Resource* resource = nullptr;
	ResourceTexture* textureIcon = nullptr;

	TextEditor editor;
	std::string fileToEdit;

	uint iconSize = 80;

	char title[25];
	char label[32];
	int window_width = 0;
	int window_height = 0;
	float brightness = 0;
	int fpsCap = 0;
	bool dockingWindow = true;

	bool show_demo_window = false;
	bool show_configuration_window = true;
	bool show_about_window = false;
	bool show_console_window = true;
	bool show_hierarchy_window = true;
	bool show_inspector_window = true;
	bool show_dropTarget_window = false;
	bool show_texteditor_window = false;
	bool show_saveeditor_popup = false;

	bool depthtest = false;
	bool cullface = true;
	bool lighting = true;
	bool colormaterial = false;
	bool texture2D = true;
	bool cubemap = false;
	bool polygonssmooth = false;

	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool full_desktop = false;

	bool drawNormals = false;
	bool drawWireframe = false;
	bool drawCheckerTex = false;
	bool drawTexture = true;

	bool scrollDown = true;
	bool GUIhovered = false;
	bool isUserTyping = false; 

	bool enableObject = true;

	bool textureIconLoaded = false;

	bool allowSaveOrLoad = false;

	Color frustumColor = Color(1.0f, 1.0f, 0.2f, 0.75f);
	Color NormalColor = Color(.8f, .8f, 0.0f, 0.75f);
	Color AABBColor = Color(1.0f, 0.2f, 0.2f, 0.75f);
	Color OBBColor = Color(0.2f, 0.2f, 1.0f, 0.75f);
	ImVec4 ExplorerIconsTint;
};

#endif //__ModuleEditor_H__