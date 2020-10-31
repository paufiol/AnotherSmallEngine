#ifndef __ModuleFileSystem_H__
#define __ModuleFileSystem_H__


#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>

// Folders Path
#define ASSETS_FOLDER "Assets/"
#define MODELS_FOLDER "Assets/Models/"
#define TEXTURES_FOLDER "Assets/Textures/"

using namespace std;

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
	FileDropType& ReadExtension(const string& file_path);

	void SplitFilePath(const char* full_path, string* path, string* file = nullptr, string* extension = nullptr) const;
	void NormalizePath(char* full_path) const;
	void NormalizePath(string& full_path) const;

	bool StringCmp(const char* str1, const char* str2);
};


#endif //__ModuleFileSystem_H__