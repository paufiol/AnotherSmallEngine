#ifndef __RESOURCEMODEL_H__
#define __RESOURCEMODEL_H__

#include "Globals.h"
#include "Resource.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Math/Quat.h"

class GameObject;
struct ModelContainer
{
	ModelContainer(uint32 ID, const char* name = "", const float3& position = float3::zero, const float3& scale = float3::one, const Quat& rotation = Quat::identity, uint32 parentID = 0)
		: name(name),
		ID(ID),
		parentID(parentID),
		materialID(-1),
		meshID(-1),
		position(position),
		scale(scale),
		rotation(rotation)
	{}


	std::string name;
	uint ID;
	uint parentID;
	int meshID;
	int materialID;
	float3 position;
	float3 scale;
	Quat rotation;
};


class ResourceModel : public Resource
{
public:
	ResourceModel();
	ResourceModel(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID);

	~ResourceModel();

public:

	std::vector<ModelContainer> models;
};

#endif //__RESOURCEMODEL_H__
