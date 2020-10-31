#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	parent->AddComponent(this);
};

void ComponentMesh::Enable() {

}

void ComponentMesh::Disable() {

}

void ComponentMesh::Update() {

}