#pragma once
#include "Module.h"
#include "Globals.h"
#include "I_Model.h"

using namespace std;

class Primitive;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void LoadModel(const char* path);


	I_Model* model;
private:

	std::vector<Primitive*> primitives;
	uint buffer;

};
