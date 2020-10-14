#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"
#include "Globals.h"

struct MeshInfo
{
	uint id_index;
	uint num_index;
	uint* index = nullptr;

	uint id_vertex;
	uint num_vertex;
	float* vertex = nullptr;
};


class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Start() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	void LoadMeshInfo(const char* file_path);

public:

	std::vector<MeshInfo> mesh_list;

};

#endif //__ModuleImporter_H__
