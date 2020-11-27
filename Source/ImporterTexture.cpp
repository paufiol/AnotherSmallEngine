#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ImporterTexture.h"

#include"ResourceMaterial.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleScene.h"


#include "Dependencies/Devil/Include/ilut.h"
#include "Dependencies/Devil/Include/ilu.h"
#include "Dependencies/Devil/Include/il.h"

#pragma comment (lib, "Dependencies/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Dependencies/Devil/libx86/ILU.lib")
#pragma comment (lib, "Dependencies/Devil/libx86/ILUT.lib")

void Importer::TextureImporter::InitDevil()
{
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
}

ResourceMaterial* Importer::TextureImporter::ImportTexture(const char* path)
{  
    ILuint Il_Tex;
    uint tempid;
    ilGenImages(1, &Il_Tex);
    ilBindImage(Il_Tex);
    ilLoadImage(path);
    tempid = ilutGLBindTexImage();
    ilDeleteImages(1, &Il_Tex);
    if (Il_Tex != NULL)
    {
        LOG("Successfuly loaded %s texture", path);
    }
    else {
        LOG("Error loading the texture!");
    }
    Texture tempTex;
    tempTex.id = tempid;
    tempTex.path = path;
    ResourceMaterial* tempMat = new ResourceMaterial(tempTex);

    return tempMat;
}

