#ifndef __RESOURCESCENE_H__
#define __RESOURCESCENE_H__

#include "Globals.h"
#include "Resource.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Math/Quat.h"

class ResourceScene : public Resource
{
public:
	ResourceScene();
	ResourceScene(const char* assetsFile,const char* libraryFile, const char* name , uint32 UID);

	~ResourceScene();

public:

};

#endif //__RESOURCESCENE_H__