#include "ResourceScene.h"
#include "GameObject.h"

ResourceScene::ResourceScene() : Resource(ResourceType::Scene)
{
	rootObject = new GameObject("Scene");
	//models.clear();
}

ResourceScene::ResourceScene(const char* assetsFile,const char* libraryFile, const char* name) : Resource(ResourceType::Scene)
{
	this->name = name;
	this->libraryFile = libraryFile;
	this->assetsFile = assetsFile;

}

ResourceScene::~ResourceScene()
{
}
