#include "Application.h"
#include "ModuleImporter.h"

#include "Dependencies/Assimp/Assimp/include/cimport.h"
#include "Dependencies/Assimp/Assimp/include/scene.h"
#include "Dependencies/Assimp/Assimp/include/postprocess.h"
#include "Dependencies/Assimp/Assimp/include/mesh.h"
#pragma comment (lib, "Dependencies/Assimp/Assimp/libx86/assimp.lib")

ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{

}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Start()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

update_status ModuleImporter::Update(float dt)
{
	
	
	
	
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

void ModuleImporter::LoadMeshInfo(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			MeshInfo* newMesh = new MeshInfo();
			if (scene->mMeshes[i]->HasFaces())
			{
				for (uint f = 0; f < scene->mMeshes[i]->mNumFaces; ++f)
				{
					if (scene->mMeshes[i]->mFaces[f].mNumIndices != 3)
					{
						LOG("WARNING, geometery face with != 3 indices!");
					}
					else
					{
						memcpy(&newMesh->index[f * 3], scene->mMeshes[i]->mFaces[f].mIndices, 3 * sizeof(uint));
					}

				}
			}
		}
		
		aiReleaseImport(scene);
	}
	//else
		//LOG(“Error loading scene % s”, file_path);
}






