#pragma once
#include "Module.h"
#include "Globals.h"


using namespace std;

class GameObject;
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

	
private:

	std::vector<GameObject*> game_objects;
	std::vector<Primitive*> primitives;

	GameObject* root_object;
};
