#include "Application.h"
#include "ImporterShader.h"
#include "ResourceShader.h"
#include "ModuleFileSystem.h"

#include "Dependencies/Glew/include/GL/glew.h"
#include "Dependencies/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "Dependencies/Glew/libx86/Release/Win32/glew32.lib") 


void Importer::ShaderImporter::Import(const char* fullPath, ResourceShader* shader)
{
	std::string path;
	std::string name;
	std::string ext;
	App->fileSystem->SplitFilePath(fullPath, &path, &name, &ext);

	char* buffer;
	int size = App->fileSystem->Load(fullPath, &buffer);

	if (size <= 0)
	{
		delete[] buffer;
		LOG("Shader: %s not found or can't be loaded.", fullPath);
		return;
	}
	
	std::string file(buffer);
	if (file.find(VERTEX_SHADER) != std::string::npos)
	{
		shader->vertexID = ImportVertex(file, shader);
	}
	if (file.find(FRAGMENT_SHADER) != std::string::npos)
	{
		shader->fragmentID = ImportFragment(file, shader);
	}

	delete[] buffer;

	// Create the Shader Program and link it
	if (shader->vertexID != 0 && shader->fragmentID != 0)
	{
		GLint outcome;
		shader->vertexID = (GLuint)shader->vertexID;
		shader->fragmentID = (GLuint)shader->fragmentID;

		shader->shaderProgramID = glCreateProgram();
		glAttachShader(shader->shaderProgramID, shader->vertexID);
		glAttachShader(shader->shaderProgramID, shader->fragmentID);
		glLinkProgram(shader->shaderProgramID);

		glGetProgramiv(shader->shaderProgramID, GL_LINK_STATUS, &outcome);
		if (outcome == 0)
		{
			GLchar info[512];
			glGetProgramInfoLog(shader->shaderProgramID, 512, NULL, info);
			LOG("Shader compiling error: %s", info);
		}

		glDeleteShader(shader->vertexID);
		glDeleteShader(shader->fragmentID);
		glDeleteShader(shader->shaderProgramID);
	}
	else
	{
		LOG("ERROR, Vertex shader: &d or Fragment shader: %d are not correctly compiled.", shader->vertexID, shader->fragmentID);
	}
	

}

int Importer::ShaderImporter::ImportVertex(std::string shaderFile, ResourceShader* shader)
{
	std::string vertexFile = std::string ("#version 330 core\r\n") + 
		std::string("#define ") +
		VERTEX_SHADER + "\r\n";
	vertexFile += shaderFile;

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = (const GLchar*)vertexFile.c_str();
	glShaderSource(vertexShader, 1, &source, nullptr);
	glCompileShader(vertexShader);
	
	GLint outcome;
	GLchar info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		LOG("Vertex shader compilation error (%s)", info);
	}

	

	return (outcome != 0) ? vertexShader : -1;
}

int Importer::ShaderImporter::ImportFragment(std::string shaderFile, ResourceShader* shader)
{
	std::string fragmentFile = std::string("#version 330 core\r\n") +
		std::string("#define ") + 
		FRAGMENT_SHADER + "\r\n";
	fragmentFile += shaderFile;
	
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* source = (const GLchar*)fragmentFile.c_str();
	glShaderSource(fragmentShader, 1, &source, nullptr);
	glCompileShader(fragmentShader);

	GLint outcome;
	GLchar info[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		LOG("Vertex shader compilation error (%s)", info);
	}
 

	return (outcome != 0) ? fragmentShader : -1;
}

uint64 Importer::ShaderImporter::Save(const ResourceShader* shader, char** buffer)
{
	char* binaryBuffer = nullptr;
	GLint binarySize = 0;
	int fullSize = 0;

	glGetProgramiv(shader->shaderProgramID, GL_PROGRAM_BINARY_LENGTH, &binarySize);

	if(binarySize > 0)
	{
		fullSize = sizeof(uint32) + binarySize;
		*buffer = new char[fullSize];
		binaryBuffer = new char[binarySize];

		GLsizei lenght = 0;
		GLenum binaryFormat;

		glGetProgramBinary(shader->shaderProgramID, binarySize, &lenght, &binaryFormat, binaryBuffer);

		char* cursor = *buffer;
		memcpy(cursor, &binaryFormat, sizeof(uint32));
		cursor += sizeof(uint32);

		memcpy(cursor, binaryBuffer, binarySize);
		RELEASE_ARRAY(binaryBuffer);
	}
		
	return fullSize;
}

void Importer::ShaderImporter::Load(ResourceShader* shader, const char* buffer, uint size)
{
	GLenum binaryFormat;

	const char* cursor = buffer;
	memcpy(&binaryFormat, cursor, sizeof(uint32));
	cursor += sizeof(uint32);

	shader->shaderProgramID = glCreateProgram();
	glProgramBinary(shader->shaderProgramID, binaryFormat, cursor, size - sizeof(uint32));


	GLint outcome = 0;
	GLchar info[512];
	glGetShaderiv(shader->shaderProgramID, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(shader->shaderProgramID, 512, NULL, info);
		LOG("Vertex shader compilation error (%s)", info);

		glDetachShader(shader->shaderProgramID, shader->vertexID);
		glDetachShader(shader->shaderProgramID, shader->fragmentID);
		glDeleteProgram(shader->shaderProgramID);

		Importer::ShaderImporter::Import(shader->assetsFile.c_str(), shader);
	}



}
