#include "GameObject.h"
#include "Component.h"

GameObject::GameObject()
{
	Enable();
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	if (active)
	{
		//Update Components --------------------------
		for (uint i = 0; i < components.size(); i++)
		{
			if (components[i]->isActive())
			{
				components[i]->Update();
			}
		}

		//Update child Game Objects -------------------
		for (uint i = 0; i < childs.size(); i++)
		{
			if (childs[i]->isActive())
			{
				childs[i]->Update();
			}
		}
	}
}

bool GameObject::Enable()
{
	if (!active)
	{
		active = true;
	}
	return active;
}

bool GameObject::Disable()
{
	if (active)
	{
		active = false;
	}
	return active;
}

bool GameObject::isActive() const
{
	return active;
}

Component * GameObject::FindComponentByType(Comp_Type type)
{
	Component* comp = nullptr;

	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type) //We need to check if the component is ACTIVE first?¿
		{
			comp = components[i];
			break;
		}
	}

	return comp;
}

Component * GameObject::CreateComponent(Comp_Type type)
{
	Component* comp = nullptr;
	bool dupe = false;
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type) //We need to check if the component is ACTIVE first?¿
		{
			dupe = true;
			break;
		}
	}

	if (!dupe)
	{
		Component* comp = new Component(type);
		components.push_back(comp);
	}

	return comp;
}
