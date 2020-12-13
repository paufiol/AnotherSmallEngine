#include "Application.h"
#include "ModuleFileSystem.h"

#include "ModuleResource.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceScene.h"
#include "ResourceFolder.h"

#include "ImporterMaterials.h"
#include "ImporterMesh.h"
#include "ImporterScene.h"
#include "ImporterTexture.h"

#include "JsonConfig.h"
#include "PathNode.h"

#include "Dependencies/Assimp/include/scene.h"

ModuleResources::ModuleResources(bool start_enabled) : Module(start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Start()
{
	LoadAssets();
	
	return true;
}

bool ModuleResources::CleanUp()
{
	return false;
}

void ModuleResources::LoadAssets()
{
	std::vector<std::string> extension;
	//extension.push_back("meta");
	uint32 modelFolder = 0;
	PathNode models = App->fileSystem->GetAllFiles(ASSETS_FOLDER, nullptr, &extension);
	IterateAssets(models, modelFolder);
	
}

bool ModuleResources::IterateAssets(PathNode node, uint32 ID)
{
	bool isNew = true;
	std::string metaPath = node.path + ".meta";
	//if (App->fileSystem->Exists(metaPath.c_str()))
	//{
	//	char* buffer = nullptr;
	//	uint size = App->fileSystem->Load(metaPath.c_str(), &buffer);
	//	if (size > 0)
	//	{
	//		JsonConfig jsonMeta(buffer);

	//		std::map<uint32, Resource*>::iterator iterator = importedResources.find(jsonMeta.GetNumber("UID"));
	//		if (iterator != importedResources.end())
	//		{
	//			ID = iterator->first;
	//			ImportFile(node.path.c_str()); //Not sure if this is the right approach
	//		}
	//		else
	//		{
	//			string name = jsonMeta.GetString("Name");
	//			uint32 UID = jsonMeta.GetNumber("UID");
	//			ResourceType type = GetTypefromString(jsonMeta.GetString("Type"));
	//			Resource* resource = new Resource(type,node.path.c_str(), name.c_str(), UID);
	//			resource->libraryFile = jsonMeta.GetString("Library file");
	//			importedResources[resource->UID] = resource;
	//			ID = resource->UID;
	//			
	//			//-------------------------
	//			if (resource->type == ResourceType::Model)
	//			{
	//				char* buffer = nullptr;
	//				uint size = App->fileSystem->Load(resource->GetLibraryFile().c_str(), &buffer);
	//				JsonConfig jsonModels(buffer); //needs buffer??
	//				ArrayConfig jsonArrray = jsonModels.GetArray("Models");
	//				for (uint i = 0; i < jsonArrray.GetSize(); i++)
	//				{
	//					JsonConfig model = jsonArrray.GetNode(i);
	//					name = model.GetString("Name");

	//					uint meshUID = model.GetNumber("Mesh UID");
	//					uint materialUID = model.GetNumber("Material UID");
	//					if (meshUID != 0)
	//					{
	//						Resource* newResource = CreateNewResource(node.path.c_str(), ResourceType::Mesh, name.c_str(), meshUID);
	//						LoadResource(meshUID, newResource);
	//						/*Resource* newResource = new Resource(ResourceType::Mesh, node.path.c_str(), name.c_str(), meshUID);
	//						newResource->libraryFile = MESHES_PATH;
	//						newResource->libraryFile.append(std::to_string(resource->GetUID()));
	//						newResource->libraryFile.append(ASE_EXTENSION);*/
	//						//importedResources[newResource->UID] = newResource;
	//					}

	//					if (materialUID != 0)
	//					{
	//						Resource* newResource = CreateNewResource(node.path.c_str(), ResourceType::Material, name.c_str(), materialUID);
	//						LoadResource(materialUID, newResource);
	//						//Resource* newResource = new Resource(ResourceType::Material, node.path.c_str(), name.c_str(), materialUID);
	//						//newResource->libraryFile = MATERIALS_PATH;
	//						//newResource->libraryFile.append(std::to_string(resource->GetUID()));
	//						//newResource->libraryFile.append(ASE_EXTENSION);
	//						//importedResources[newResource->UID] = newResource;
	//					}
	//				}	
	//			}

	//			LoadResource(ID, resource); //To delete once we have the imgui window

	//			
	//		}
	//	}
	//	RELEASE_ARRAY(buffer);
	//}
	//else
	//{
	//	ID = ImportFile(node.path.c_str());
	//	if (ID != 0)
	//	{
	//		LOG("Succesfully imported from: %s", node.path.c_str());
	//	}
	//	else if (ID == 0)
	//	{
	//		LOG("ERROR: Couldn't import the file: %s", node.path.c_str());
	//	}
	//}
	ID = ImportFile(node.path.c_str());
	if (ID != 0)
	{
		LOG("Succesfully imported from: %s", node.path.c_str());
	}
	else if (ID == 0)
	{
		LOG("ERROR: Couldn't import the file: %s", node.path.c_str());
	}

	if (!node.isFile && !node.isLeaf)
	{
		std::vector<uint32> newChildren;
		for (uint i = 0; i < node.children.size(); i++)
		{
			uint32 childID = 0;
			if (IterateAssets(node.children[i], childID))
			{
				newChildren.push_back(childID);
			}
		}
	}

	return isNew;
}



uint32 ModuleResources::Find(const char* file)
{
	return uint32();
}

uint32 ModuleResources::ImportFile(const char* assetsFile)
{
	ResourceType type = GetTypeFromFile(assetsFile);

	Resource* resource = CreateNewResource(assetsFile, type);
	char* buffer = nullptr;
	uint64 fileSize = 0;
	if (type != ResourceType::Folder)
		fileSize = App->fileSystem->Load(assetsFile, &buffer);

	switch (type)
	{
	case ResourceType::Texture: 
		Importer::TextureImporter::ImportTexture((ResourceTexture*)resource, buffer, fileSize);
		SaveResource((ResourceTexture*)resource);
		break;
	case ResourceType::Scene: 
		 
		LoadScene(buffer, fileSize, (ResourceScene*)resource);
		SaveResource((ResourceScene*)resource);

		break;
	case ResourceType::Model:
		
		LoadScene(buffer, fileSize, (ResourceScene*)resource);
		SaveResource((ResourceScene*)resource);

		//LOAD THE MODEL/SCENE This is here for now
		LoadResource(resource->GetUID(), resource);
		break;

	}
	

	RELEASE_ARRAY(buffer);
	//UnloadResource(resource->GetUID());

	return resource->GetUID();
}

void ModuleResources::LoadScene(const char* buffer, uint size, ResourceScene* scene)
{
	//ResourceScene* resourceScene = new ResourceScene(scene->assetsFile.c_str(),scene->libraryFile.c_str(),scene->name.c_str(),scene->UID);

	const aiScene* aiScene = Importer::ModelImporter::ImportAssimpScene(buffer, size);
	Importer::ModelImporter::ImportScene(aiScene, scene);

	std::vector<uint32> meshes;

	for (uint i = 0; i < aiScene->mNumMeshes; i++)
	{
		std::string name;
		for (uint h = 0; h < scene->models.size(); h++)
		{
			if (scene->models[h].meshID == i) name = scene->models[h].name;
		}
		aiScene->mMeshes[i]->mName = name;
		ResourceMesh* resourceMesh = (ResourceMesh*)CreateNewResource(scene->GetAssetsFile().c_str(), ResourceType::Mesh, aiScene->mMeshes[i]->mName.C_Str());
		
		Importer::MeshImporter::LoadMeshes(resourceMesh, aiScene->mMeshes[i]);
		SaveResource(resourceMesh);

		meshes.push_back(resourceMesh->GetUID());

		//char* newbuffer = nullptr;
		//size = App->fileSystem->Load(resourceMesh->GetLibraryFile().c_str(), &newbuffer);
		//Importer::MeshImporter::Load(resourceMesh, newbuffer);
		//resourceMesh->SetUpBuffers(resourceMesh);

		//SaveResource(resource);


	}
	std::vector<uint32> materials;
	for (uint i = 0; i < aiScene->mNumMaterials; i++)
	{
		std::string name;
		aiScene->mMaterials[i]->Get(AI_MATKEY_NAME, name);
			
		ResourceMaterial* resourceMaterial = (ResourceMaterial*)CreateNewResource(scene->GetAssetsFile().c_str(), ResourceType::Material, aiScene->mMeshes[i]->mName.C_Str());


		Importer::MaterialsImporter::ImportMaterial(aiScene->mMaterials[i], resourceMaterial);
		SaveResource(resourceMaterial);
		materials.push_back(resourceMaterial->GetUID());

		//char* newbuffer = nullptr;
		//size = App->fileSystem->Load(resourceMaterial->GetLibraryFile().c_str(), &newbuffer);
		//Importer::MaterialsImporter::Load(resourceMaterial, newbuffer);
		
	}

	for (uint i = 0; i < scene->models.size(); i++)
	{
		if (scene->models[i].meshID != -1)
		{
			scene->models[i].meshID = meshes[scene->models[i].meshID];
		}
		else
		{
			scene->models[i].meshID = 0;
		}
		if (scene->models[i].materialID != -1)
		{
			scene->models[i].materialID = materials[scene->models[i].materialID];
		}
		else
		{
			scene->models[i].materialID = 0;
		}
	}

}


void ModuleResources::ReleaseResource(uint32 UID)
{
}

Resource* ModuleResources::CreateNewResource(const char* assetsFile, ResourceType type, const char* name, uint32 UID)
{
	Resource* resource = nullptr;
	
	if (name == nullptr)
	{
		LOG("Unknown resource Name");
	}
	switch (type)
	{
	case ResourceType::Texture: 
		resource = new ResourceMaterial(assetsFile, TEXTURES_PATH, name, UID);
		break;
	case ResourceType::Mesh:

		resource = new ResourceMesh(assetsFile, MESHES_PATH, name, UID);
		break;
	case ResourceType::Scene:

		resource = new ResourceScene(assetsFile, SCENES_PATH, name, UID);
		break;
	case ResourceType::Material:

		resource = new ResourceMaterial(assetsFile, MATERIALS_PATH, name, UID);
		break;
	case ResourceType::Folder:
		resource = new ResourceFolder(assetsFile, FOLDERS_PATH, name, UID);
		
		break;
	case ResourceType::Model:
		resource = new ResourceScene(assetsFile, MODELS_PATH, name, UID);
		break;
	default:
		resource = new Resource();
		break;
	}

	if (resource->type != ResourceType::None)
	{
		resource->libraryFile.append(std::to_string(resource->GetUID()));
		resource->libraryFile.append(ASE_EXTENSION);
		importedResources[resource->UID] = resource;
	}

	return resources[resource->UID] = resource;

}

void ModuleResources::SaveMeta(Resource* resource)
{
	JsonConfig jsonConfig;
	jsonConfig.SetNumber("UID", resource->GetUID());
	jsonConfig.SetString("Name", resource->name.c_str());
	jsonConfig.SetString("Library file", resource->GetLibraryFile());
	jsonConfig.SetString("Type", GetStringFromResource(resource).c_str());
	std::string path = resource->GetAssetsFile().append(".meta");
	char* buffer;
	uint size = jsonConfig.SerializeConfig(&buffer);
	if (size > 0)
	{
		App->fileSystem->Save(path.c_str(), buffer, size);
	}
}

void ModuleResources::SaveResource(Resource* resource)
{
	char* buffer = nullptr;
	uint size = 0;

	switch (resource->GetType())
	{
	//case(ResourceType::FOLDER): { size = Importer::Folders::Save((R_Folder*)resource, &buffer); break; }
	case(ResourceType::Mesh): { size = Importer::MeshImporter::Save((ResourceMesh*)resource, &buffer); break; }
	case(ResourceType::Texture): { size = Importer::TextureImporter::Save((ResourceTexture*)resource, &buffer); break; }
	case(ResourceType::Material): { size = Importer::MaterialsImporter::Save((ResourceMaterial*)resource, &buffer); break; }
	case(ResourceType::Scene): { size = Importer::ModelImporter::Save((ResourceScene*)resource, &buffer);break; }
	case(ResourceType::Model):{ size = Importer::ModelImporter::Save((ResourceScene*)resource, & buffer); break; }
	}

	if (size > 0)
	{
		SaveMeta(resource);
		App->fileSystem->Save(resource->GetLibraryFile().c_str(), buffer, size);

		RELEASE_ARRAY(buffer);
	}
}

void ModuleResources::LoadResource(uint32 UID, Resource* resource)
{
	//Resource* tempResource = CreateNewResource(resource->assetsFile.c_str(),resource->GetType(),resource->name.c_str());
	char* buffer = nullptr;
	uint size = App->fileSystem->Load(resource->GetLibraryFile().c_str(), &buffer);
	if (size == 0)
	{
		LOG("ERROR The resource doen't have a library file");
		RELEASE(resource);
		return ;
	}
	switch (resource->GetType())
	{
	case (ResourceType::Mesh):
	{
		
		Importer::MeshImporter::Load((ResourceMesh*)resource, buffer);
		
		break;
	}
	case (ResourceType::Material):
	{
		Importer::MaterialsImporter::Load((ResourceMaterial*)resource, buffer);
		break;
	}
	case (ResourceType::Model):
	{
		Importer::ModelImporter::Load((ResourceScene*)resource, buffer);
		break;
	}
	case(ResourceType::Scene):
	{
		Importer::ModelImporter::Load((ResourceScene*)resource, buffer);
		break;
	}
	case(ResourceType::Texture):
		Importer::TextureImporter::Load((ResourceTexture*)resource, buffer,size);
		break;
	}
	

	//Add to the resources list!!!!!!!!!!

	/*return resource;*/
}

Resource* ModuleResources::AccesResource(uint32 UID)
{
	LOG("Loading: %d", UID);

	std::map<uint32, Resource*>::iterator it = resources.find(UID);
	if (it != resources.end())
	{
		return it->second;
	}
	std::map<uint32, Resource*>::iterator _it = importedResources.find(UID);
	if (_it != importedResources.end())
	{
		LoadResource(UID,_it->second);
	}
	return nullptr;
}




ResourceType ModuleResources::GetTypeFromFile(const char* path) const
{
	std::string extension;
	App->fileSystem->SplitFilePath(path, nullptr, nullptr, &extension);

	static_assert(static_cast<int>(ResourceType::None) == 6, "Code Needs Update");

	/*if (extension == "FBX" || extension == "fbx")
		return ResourceType::MODEL;*/
	if (extension == "tga" || extension == "png" || extension == "jpg" || extension == "TGA" || extension == "PNG" || extension == "JPG")
		return ResourceType::Texture;
	if (extension == "scene")
		return ResourceType::Scene;
	if (extension == "FBX" || extension == "fbx")
		return ResourceType::Model;


	return App->fileSystem->IsDirectory(path) ? ResourceType::Folder : ResourceType::None;
}

ResourceType ModuleResources::GetTypefromString(std::string typeString) const
{
	if (typeString == "Folder") return ResourceType::Folder;
	else if (typeString == "Material") return ResourceType::Material;
	else if (typeString == "Mesh") return ResourceType::Mesh;
	else if (typeString == "Model") return ResourceType::Model;
	else if (typeString == "Scene") return ResourceType::Scene;
	else if (typeString == "Texture") return ResourceType::Texture;
	return ResourceType::None;
}

std::string ModuleResources::GetStringFromResource(Resource* resource)
{
	string path;
	switch (resource->type)
	{
	case ResourceType::Folder:
		return "Folder";
		break;
	case ResourceType::Material:
		//App->fileSystem->GetRealDir(resource->libraryFile.c_str(), path);
		App->fileSystem->SplitFilePath(resource->libraryFile.c_str(), &path, nullptr, nullptr);
		if (path == TEXTURES_PATH)
		{
			return "Texture";
		}

		else return "Material";
		break;
	case ResourceType::Mesh:
		return "Mesh";
		break;
	case ResourceType::Model:
		return "Model";
		break;
	case ResourceType::Scene:
		//return "Scene";
		return "Model";			//THIS IS BAD, shoudn't be done 
		break;
	case ResourceType::Texture:
		return "Texture";
		break;
	}
	
	return "None";
}
