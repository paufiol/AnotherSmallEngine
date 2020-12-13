#ifndef __ResoruceMaterial_H__
#define __ResoruceMaterial_H__

#include "Globals.h"
#include "Color.h"
#include "Resource.h"
#include "ResourceTexture.h"

//struct Texture
//{
//	uint id = 0;
//	std::string path;
//};



class ResourceMaterial : public Resource
{
public:

	ResourceMaterial(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID);
	ResourceMaterial(uint _id, const char* _path);
	ResourceMaterial(ResourceTexture* texture);
	ResourceMaterial();
	~ResourceMaterial();



	inline ResourceTexture* GetTexture() { return texture; }
	inline Color GetColor() const { return color; }
	inline uint GetId()  { return texture->id; }
	inline std::string GetPath() const { return texture->path; }
	
	inline void SetColor(Color color) { this->color = color; }

	inline void SetTexture(ResourceTexture* rTexture) { this->texture = rTexture; }
	inline void SetTexture(uint id, const char* path) { this->texture->id = id; this->texture->path = path; }

	inline void SetId(uint id) { this->texture->id = id; }
	inline void SetPath(std::string path) 
	{ this->texture->path = path; }


private:

	ResourceTexture* texture = nullptr;
	Color color = Color(1.0f,1.0f,1.0f,1.0f);

};
#endif //__ResoruceMaterial_H__