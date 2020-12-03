#ifndef __TextureImporter_H__
#define __TextureImporter_H__

#include "Globals.h"
#include "OpenGL.h"

using namespace std;

class ResourceMaterial;

namespace Importer
{

	namespace TextureImporter
	{
		ResourceMaterial* ImportTexture(const char* file);
		void InitDevil();
	}
}




#endif //__TextureImporter_H__
