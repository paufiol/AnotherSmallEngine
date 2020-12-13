#ifndef _MODULE_RESOURCES_H_
#define _MODULE_RESOURCES_H_

#include "Module.h"
#include "Resource.h"
#include <map>

class Random;
class ResourceMesh;
class ResourceTexture;
class ResourceScene;

struct PathNode;

class ModuleResources : public Module
{
public:
	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Start() override;
	//update_status Update(float dt) override;
	bool CleanUp() override;

	void LoadAssets();
	bool IterateAssets(PathNode node, uint32 ID);
	void LoadScene(const char* buffer, uint size, ResourceScene* scene);
	uint32 Find(const char* file);
	uint32 ImportFile(const char* file_assets);
	ResourceType GetTypeFromFile(const char* path) const;
	ResourceType GetTypefromString(std::string) const;
	std::string GetStringFromResource(Resource* resource);

	Resource* CreateNewResource(const char* assetsFile, ResourceType type, const char* name = "", uint32 UID = 0);
	void SaveMeta(Resource* resource);
	void SaveResource(Resource* resource);
	Resource* LoadResource(uint32 UID, Resource* resource = nullptr);
	Resource* AccesResource(uint32 UID);
	void ReleaseResource(uint32 UID);
	//uint32 UnloadResource(uint32 ID);

	inline uint32 GenerateNewUID() { return randomNum.GenerateRandomInt(); }

public:

	std::map<uint32, Resource*> resources;

private:

	std::map<uint32, Resource*> importedResources;


	Random randomNum;
};


#endif // !_MODULE_RESOURCES_H_
