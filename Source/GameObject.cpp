#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ResourceMesh.h"
#include "Globals.h"


#include "ModuleResource.h"
#include "ResourceScene.h"

GameObject::GameObject(std::string name): name(name), active(true), 
UID(randomNum.GenerateRandomInt())
{
	AddComponent(new ComponentTransform(this));
	this->parent = nullptr;	// May crash HERE, at the moment it's necesary
	
	aabb.SetNegativeInfinity();
	obb.SetNegativeInfinity();
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

		for (; item != components.end() && ret == true; ++item) 
		{
			(*item)->Update();

			float4x4 global_parent = float4x4::identity;
			if (transform && transform->updatedtransform)
			{
				if (parent) 
				{
					ComponentTransform* tmp = (ComponentTransform*)parent->GetComponent(ComponentType::Transform);
					(*item)->OnUpdateTransform(transform->GetGlobalTransform(), tmp->GetGlobalTransform());
				}
				else
					(*item)->OnUpdateTransform(transform->GetGlobalTransform());
				
				UpdateAABB();

				if (!children.empty())
				{	
					std::vector<GameObject*>::iterator children_i = children.begin();
					for (; children_i != children.end(); children_i++) {
						(*children_i)->transform->updatedtransform = true;
					}
				}
			}
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

const uint32 GameObject::GetUID() const
{
	return UID;
}

void GameObject::SetUID(uint32 UID)
{
	this->UID = UID;
}

const AABB& GameObject::GetAABB() const
{
	return aabb;
}

const OBB& GameObject::GetOBB() const
{
	return obb;
}

void GameObject::UpdateAABB()
{
	//C_Mesh* mesh = GetComponent <C_Mesh>();
	std::vector<Component*> meshes = GetComponents(ComponentType::Mesh);
	
	if (!meshes.empty()) 
	{
		for (uint i = 0; i < meshes.size(); ++i)
		{
			//Weird shenanigans because GetComponents should be a template return 
			ComponentMesh* temp_mesh = (ComponentMesh*)meshes[i];
			if (temp_mesh->GetMesh() != nullptr)
			{
				AABB temp_aabb = temp_mesh->GetAABB();
				obb = temp_mesh->GetAABB();
			}


			ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::Transform);
			obb.Transform(transform->GetGlobalTransform());

			aabb.SetNegativeInfinity();
			aabb.Enclose(obb);
		}
	}
	else
	{
		aabb.SetNegativeInfinity();
		aabb.SetFromCenterAndSize(transform->GetPosition(), float3::one);
		obb = aabb;
	}
}

bool GameObject::IsSelected()
{
	if (this->name == App->scene->selected_object->name) return true;
	else return false;
}

bool GameObject::IsRootObject()
{
	if (this->name == App->scene->root_object->name) return true;
	else return false;
}

Component* GameObject::AddComponent(Component* component)
{

	switch (component->type) //This could probably be done with a template Component or smth
	{
	case(ComponentType::Error):
		LOG("Component Type Error! Something broke...");
		
		break;
	case(ComponentType::Transform):
		transform = (ComponentTransform*)component;
		break;
	case(ComponentType::Material):
		if (this->GetComponent(ComponentType::Material) != nullptr) {
			return nullptr;
		}
		texture = (ComponentTexture*)component;
		break;
	case(ComponentType::Mesh):
		break;
	case(ComponentType::Camera):
		break;
	}
	
	//components.push_back(ret);
	components.push_back(component);
	return component;
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

		while (parentObject != App->scene->root_object)	//Iterate all parents to avoid parenting a parent to its own child
		{
			if (parentObject == children)
			{
				LOG("ERROR: Can't add %s to %s, they are already parented", children->name.c_str(), this->name.c_str());
				return nullptr;
			}
			else if (parentObject->parent != nullptr)
			{
				parentObject = parentObject->parent;
			}
		}
	}
	
	if (children->parent != nullptr) 
	{
		children->parent->EraseChild(children, false); //EraseChild Deletes the Child right now
		children->SetParent(this);
	}
	else if (children->parent == nullptr)
	{
		children->SetParent(this);
	}

	this->transform->updatedtransform = true;

	this->children.push_back(children);
	return children;
}

void GameObject::EraseChild(GameObject* child, bool deleteChild)
{
	if (!children.empty())
	{
		for (uint i = 0; i < children.size(); ++i)
		{
			if (children[i] == child)
			{
				if (deleteChild)
				{
					children[i]->CleanUp();
				
					delete children[i];
				}
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

			for (uint j = 0; j < App->scene->game_objects.size(); j++)
			{
				if (children[i] == App->scene->game_objects[j]) {
					App->scene->game_objects.erase(App->scene->game_objects.begin() + j);
					break;
				}
			}
			
			children[i]->CleanUp();

			delete children[i];
		}
		children.clear();
	}
}

void GameObject::FillGameObjectArray(GameObject* gameObject, std::vector<GameObject*> array)
{
	if (gameObject != nullptr)
	{
		array.push_back(gameObject);

		if (gameObject->children.size() > 0)
		{
			for (int i = 0; i < gameObject->children.size(); ++i)
				FillGameObjectArray(gameObject->children[i], array);
		}

	}
}

void GameObject::CreateGOfromResource(ResourceScene* model)
{

}

Component* GameObject::GetComponent(ComponentType type)
{

	for (uint i = 0; i < components.size(); ++i) {

		if (components[i]->type == type)
		{
			if(components[i]->type == ComponentType::Mesh)
			{
				ComponentMesh* comp_mesh = (ComponentMesh*)components[i];
				//if (comp_mesh->GetMesh()->size[comp_mesh->GetMesh()->index] < 20) continue; //Just to ignore the useless mesh that exists in certain buildings
			}
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