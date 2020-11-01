#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleImporter.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"
#include "ComponentTexture.h"
#include "GameObject.h"


ModuleFileSystem::ModuleFileSystem() {};

ModuleFileSystem::~ModuleFileSystem() {};

void ModuleFileSystem::ReadDropFile(const char* file_path)
{
	LOG("File Dropped with path %s", file_path);

	string final_path;
	SplitFilePath(file_path, nullptr, &final_path); // get base file name

	FileDropType type = ReadExtension(string(file_path)); // get extension type

	switch (type) { // add location
	case FileDropType::MODEL3D:
		final_path = MODELS_FOLDER + final_path;
		break;
	case FileDropType::TEXTURE:
		final_path = TEXTURES_FOLDER + final_path;
		break;
	}

	string normalized = file_path;
	NormalizePath(normalized);

	switch (type) { // call the loader
	case FileDropType::MODEL3D:
		LOG("Start Loading Model");
		Importer::MeshImporter::Import(final_path.data());
		//App->renderer3D->newMesh = Importer::MeshImporter::tempMesh;
		//App->renderer3D->LoadModel(final_path.data());
		break;
	case FileDropType::TEXTURE:
		LOG("Start Loading Texture");
		
		ComponentTexture* tempCompTex = new ComponentTexture(App->scene_intro->selected_object);
		tempCompTex->SetTexture(Importer::TextureImporter::Import(final_path.data()), final_path.data());
		
		App->scene_intro->selected_object->AddComponent(tempCompTex);
		
		//test->loadModel(final_path.data(), true);
		break;
	}

}

FileDropType& ModuleFileSystem::ReadExtension(const string& extern_path)
{

	string extension;
	SplitFilePath(extern_path.data(), nullptr, nullptr, &extension);

	FileDropType extension_type = FileDropType::DEFAULT;

	if (StringCmp(extension.data(), "fbx"))
		extension_type = FileDropType::MODEL3D;
	else if (StringCmp(extension.data(), "dds"))
		extension_type = FileDropType::TEXTURE;
	else if (StringCmp(extension.data(), "png"))
		extension_type = FileDropType::TEXTURE;
	else if (StringCmp(extension.data(), "jpg"))
		extension_type = FileDropType::TEXTURE;
	else if (StringCmp(extension.data(), "tga"))
		extension_type = FileDropType::TEXTURE;
	else
		LOG("Extension unknown!");


	return extension_type;
}

void ModuleFileSystem::SplitFilePath(const char* full_path, string* path, string* file, string* extension) const
{
	if (full_path != nullptr)
	{
		string tempPath(full_path);
		NormalizePath(tempPath);
		size_t pos_separator = tempPath.find_last_of("\\/");
		size_t pos_dot = tempPath.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < tempPath.length())
				*path = tempPath.substr(0, pos_separator + 1);
			else
				path->clear();
		}

		if (file != nullptr)
		{
			if (pos_separator < tempPath.length())
				*file = tempPath.substr(pos_separator + 1);
			else
				*file = tempPath;
		}

		if (extension != nullptr)
		{
			if (pos_dot < tempPath.length())
				*extension = tempPath.substr(pos_dot + 1);
			else
				extension->clear();
		}
	}
}
void ModuleFileSystem::NormalizePath(char* full_path) const
{
	int len = strlen(full_path);
	for (int i = 0; i < len; ++i)
	{
		if (full_path[i] == '\\')
			full_path[i] = '/';
		else
			full_path[i] = tolower(full_path[i]);
	}
}

void ModuleFileSystem::NormalizePath(string& full_path) const
{
	for (string::iterator it = full_path.begin(); it != full_path.end(); ++it)
	{
		if (*it == '\\')
			*it = '/';
		else
			*it = (*it);
	}
}

bool ModuleFileSystem::StringCmp(const char* str1, const char* str2)
{
	size_t size = strlen(str1);
	if (size != strlen(str2))
		return false;

	for (uint i = 0; i < size; ++i) {
		if (std::tolower(str1[i]) != std::tolower(str2[i]))
			return false;
	}
	return true;
}