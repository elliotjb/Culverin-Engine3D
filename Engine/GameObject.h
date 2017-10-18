#pragma once
#include "Globals.h"
#include "Component.h"
#include <string>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	bool Enable();
	bool Disable();

	bool isActive() const;

	Component* FindComponentByType(Comp_Type type);
	Component* AddComponent(Comp_Type type);

private:
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;
	GameObject* parent = nullptr;
	bool active = false;
};