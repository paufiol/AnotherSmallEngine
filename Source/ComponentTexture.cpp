#pragma once
#include "ComponentTexture.h"
#include "GameObject.h"

ComponentTexture::ComponentTexture(GameObject* parent) : Component(parent)
{
	parent->AddComponent(this);
};

void ComponentTexture::Enable() {

}

void ComponentTexture::Disable() {

}

void ComponentTexture::Update() {

}