#pragma once
#include <string>
#include <vector>

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentTexture;
enum class ComponentType;

class GameObject {
//Properties
public:
	std::string name;

	std::vector<Component*> components;
	bool active;

	GameObject* parent;
	std::vector<GameObject*> children;

	ComponentTransform* transform = nullptr;
	ComponentTexture* texture = nullptr;
private:


//Methods
public:
	GameObject(std::string name = "Game_Object");
	~GameObject();

	bool Enable(); //Start up + bool toggle
	bool Disable();//Clean up + bool toggle
	
	void Update();
	void CleanUp();

	void SetParent(GameObject* _parent);
	void SetName(const char* _name);

	//Component handling
	Component* AddComponent(Component* component);
	Component* GetComponent(ComponentType type);
	std::vector<Component*> GetComponents(ComponentType type);
	void EraseComponents();

	//Children handling
	GameObject* AddChildren(GameObject* children);
	void EraseChild(GameObject* child);	//Delete specific child
	void EraseAllChildren();			//Delete them all
	
	bool selected = false;
private:
	
};