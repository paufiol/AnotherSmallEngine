#pragma once

#include "Module.h"
#include "Globals.h"
#include <vector>

// Folders Path
#define ASSETS_FOLDER "Assets/"
#define MODELS_FOLDER "Assets/Models/"
#define TEXTURES_FOLDER "Assets/Textures/"
enum class FileDropType {
	MODEL3D,
	TEXTURE,

	DEFAULT
};

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem();

	~ModuleFileSystem();

	//bool Init();
	//update_status PreUpdate(float dt);
	//bool CleanUp() override;

	void ReadDropFile(const char* file_path);
	FileDropType& ReadExtension(const std::string& file_path);

	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;
	void NormalizePath(char* full_path) const;
	void NormalizePath(std::string& full_path) const;

	bool StringCmp(const char* str1, const char* str2);
};
