#include "Application.h"
#include "SkyBox.h"
#include "OpenGL.h"
#include "ModuleCamera3D.h"
#include "ComponentCamera.h"
#include "ModuleResource.h"
#include "ResourceShader.h"
#include "ModuleFileSystem.h"

#include "Dependencies/MathGeoLib/include/Math/float4x4.h"
#include "Dependencies/MathGeoLib/include/Math/float3x3.h"
#include "Dependencies/MathGeoLib/include/Math/TransformOps.h"

#include "Dependencies/Devil/Include/ilut.h"
#include "Dependencies/Devil/Include/ilu.h"
#include "Dependencies/Devil/Include/il.h"

#pragma comment (lib, "Dependencies/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Dependencies/Devil/libx86/ILU.lib")
#pragma comment (lib, "Dependencies/Devil/libx86/ILUT.lib")

#include <map>

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

void Skybox::SetUpSkyBoxBuffers()
{
	glGenBuffers(1, (GLuint*)&(Skybox_id));
	glBindBuffer(GL_ARRAY_BUFFER, Skybox_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, Skybox_vertices, GL_STATIC_DRAW);
}

void Skybox::CreateSkybox()
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		char* buffer = nullptr;
		std::string textureFile = SKYBOX_FOLDER;
		textureFile.append(faces[i]);
		unsigned int size = App->fileSystem->Load(textureFile.data(), &buffer);

		if (size > 0)
		{
			ILuint id;
			ilGenImages(1, &id);
			ilBindImage(id);
			if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
			{
				ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
				ILinfo info;
				iluGetImageInfo(&info);

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, info.Width, info.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.Data);

				ilDeleteImages(1, &id);
			}
		}
		delete[] buffer;
	}


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	SkyboxTex_id = textureID;
}

void Skybox::RenderSkybox()
{
	if (Skybox_programid == 0)
	{
		Skybox_programid = SetSkyboxShader();
	}
	else if (Skybox_programid != 0)
	{
		glUseProgram(Skybox_programid);
	}
	else LOG("Error loading skybox shader program");


	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	float3 translation = App->camera->currentCamera->frustum.worldMatrix.TranslatePart();
	float4x4 modelMatrix = math::float4x4::identity;
	modelMatrix.SetTranslatePart(translation);
	modelMatrix.Scale(50,50,50);

	math::float4x4 resultMatrix = modelMatrix.Transposed() * App->camera->currentCamera->GetAlternativeViewMatrix() * App->camera->currentCamera->GetAlternativeProjectionMatrix();

	GLint uinformLoc = glGetUniformLocation(Skybox_programid, "resultMatrix");
	glUniformMatrix4fv(uinformLoc, 1, GL_FALSE, *resultMatrix.v);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTex_id);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, Skybox_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);

	glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
}


uint32 Skybox::SetSkyboxShader()
{
	uint32 program_id;

	ResourceShader* resource;
	std::map<uint32, ResourceShader*> shadersInMemory = App->resources->GetShadersInMemory();
	for (std::map<uint32, ResourceShader*>::iterator item = shadersInMemory.begin(); item != shadersInMemory.end(); item++)
	{
		if (item->second->name == "SkyBoxShader")
		{
			resource = (ResourceShader*)App->resources->LoadResource(item->second->UID);
			if (resource != nullptr) return resource->shaderProgramID;
		}
	}
	return 0;
}