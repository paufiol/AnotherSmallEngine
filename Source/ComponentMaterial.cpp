#pragma once
#include "ComponentMaterial.h"
#include "GameObject.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent)
{
	parent->AddComponent(this);
};

void ComponentMaterial::Enable() {

}

void ComponentMaterial::Disable() {

}

void ComponentMaterial::Update() {

}