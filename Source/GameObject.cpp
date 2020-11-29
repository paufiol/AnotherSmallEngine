#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"
#include <vector>


GameObject::GameObject(std::string name): name(name), active(true)
{
	AddComponent(new ComponentTransform(this));
	this->parent = nullptr;	// May crash HERE, at the moment it's necesary
}

GameObject::~GameObject() 
{
	//EraseAllChildren();
	//EraseComponents();
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

void GameObject::Update()
{
	if (!components.empty()) 
	{
		std::vector<Component*>::iterator item = components.begin();
		bool ret = true;

		for (; item != components.end() && ret == true; ++item) {
			(*item)->Update();
		}
	}
}

void GameObject::CleanUp()
{
	EraseAllChildren();
	
	EraseComponents();

}

void GameObject::SetParent(GameObject* _parent)
{

	if (this != nullptr && _parent != nullptr)
	{
		this->parent = _parent;
		LOG("Game Object %s new parent: %s", this->name.c_str(), _parent->name.c_str());
	}
	else if (this == nullptr)
	{
		LOG("ERROR: Game Object %s is nullptr", this->name.c_str());
	}
	else if (_parent == nullptr)
	{
		LOG("ERROR: Game Object %s is nullptr", _parent->name.c_str());
	}

}

void GameObject::SetName(const char* _name)
{
	name = _name;
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
		transform = (ComponentTransform*)component;
		break;
	case(ComponentType::Material):
		if (this->GetComponent(ComponentType::Material) != nullptr) {
			return nullptr;
		}
		ret = new ComponentTexture(this);
		texture = (ComponentTexture*)component;
		break;
	case(ComponentType::Mesh):
		ret = new ComponentMesh(this);
		break;

	}
	
	//components.push_back(ret);
	components.push_back(component);
	return ret;
}

void GameObject::EraseComponents() 
{
	if(!components.empty())
	{
		for (uint i = 0; i < components.size(); ++i)
		{
			components[i]->CleanUp();
		}
	}
}

GameObject* GameObject::AddChildren(GameObject* children) 
{
	if (this->parent != nullptr)
	{
		GameObject* parentObject = this->parent;

		while (parentObject != App->scene_intro->root_object)	//Iterate all parents to avoid parenting a parent to its own child
		{
			if (parentObject == children)
			{
				LOG("ERROR: Can't add %s to %s, they are already parented", children->name.c_str(), this->name.c_str());
				return children;
			}
			else if (parentObject->parent != nullptr)
			{
				parentObject = parentObject->parent;
			}
		}
	}
	
	if (children->parent != nullptr) 
	{
		children->parent->EraseChild(children);
		children->SetParent(this);
	}
	else if (children->parent == nullptr)
	{
		children->SetParent(this);
	}
	this->children.push_back(children);
	return children;
}

void GameObject::EraseChild(GameObject* child)
{
	if (!children.empty())
	{
		for (uint i = 0; i < children.size(); ++i)
		{
			if (children[i] == child)
			{
				children[i]->CleanUp();
				delete children[i];
				children.erase(children.begin() + i);
			}
		}
	}
}

void GameObject::EraseAllChildren()
{
	if (!children.empty())
	{
		for (uint i = 0; i < children.size(); ++i)
		{

			for (uint j = 0; j < App->scene_intro->game_objects.size(); j++)
			{
				if (children[i] == App->scene_intro->game_objects[j]) {
					App->scene_intro->game_objects.erase(App->scene_intro->game_objects.begin() + j);
					break;
				}
			}
			
			children[i]->CleanUp();

			delete children[i];
		}
		children.clear();
	}
}

Component* GameObject::GetComponent(ComponentType type)
{
	//std::vector<Component*>::iterator item = components.begin();
	//
	//for (; item != components.end(); ++item) {

	//	if ((*item)->type == type) {
	//		return (*item);
	//	}
	//}

	for (uint i = 0; i < components.size(); ++i) {

		if (components[i]->type == type)
		{
			return(components[i]);
		}
	}

	return nullptr;
}

std::vector<Component*> GameObject::GetComponents(ComponentType type)
{
	std::vector<Component*>::iterator item = components.begin();

	std::vector<Component*> ret;

	for (; item != components.end(); ++item) {

		if ((*item)->type == type) {
			ret.push_back((*item));
		}
	}

	return ret;
}




