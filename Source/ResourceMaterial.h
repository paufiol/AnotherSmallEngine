#ifndef __ResoruceMaterial_H__
#define __ResoruceMaterial_H__

#include "Globals.h"
#include "Color.h"

struct Texture
{
	uint id = 0;
	const char* path = nullptr;
};


class ResourceMaterial
{
public:

	ResourceMaterial(uint _id, const char* _path);
	ResourceMaterial(Texture texture);
	ResourceMaterial();
	~ResourceMaterial();



	inline Texture GetTexture() { return texture; }
	inline Color GetColor() const { return color; }
	inline uint GetId() const { return texture.id; }
	inline const char* GetPath() const { return texture.path; }
	
	inline void SetColor(Color color) { this->color = color; }

	inline void SetTexture(ResourceMaterial* rMaterial) { this->texture = rMaterial->texture; }
	inline void SetTexture(uint id, const char* path) { this->texture.id = id; this->texture.path = path; }

	inline void SetId(uint id) { this->texture.id = id; }
	inline void SetPath(const char* path) { this->texture.path = path; }


private:

	Texture texture;
	Color color;

};
#endif //__ResoruceMaterial_H__