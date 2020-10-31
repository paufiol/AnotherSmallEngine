#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include <vector>


GameObject::GameObject(std::string name): name(name), active(true)
{

}

bool GameObject::Enable() //Start up + bool toggle
{
	if (!active) {
		active = true;
		return true;
		//StartUp here...
	}
	return false;
} 

bool GameObject::Disable() //Start up + bool toggle
{
	if (active) {
		active = false;
		return true;
		//CleanUp here...
	}
	return false;
}

void GameObject::Update() //Start up + bool toggle
{
	std::vector<Component*>::iterator item = components.begin();
	bool ret = true;
	
	for (; item != components.end() && ret == true; ++item) {
		//ret = (*item)->Update();
	}
}

Component* GameObject::AddComponent(Component* component)
{
	Component* ret = nullptr;

	switch (component->type)
	{
	case(ComponentType::Error):
		//LOG("Component Type Error! Something broke...");
		break;
	case(ComponentType::Transform):
		ret = new ComponentTransform(this);
		break;
	case(ComponentType::Material):
		ret = new Component(this);
		break;
	case(ComponentType::Mesh):
		ret = new Component(this);
		break;

	}
	
	components.push_back(ret);
	return ret;
}