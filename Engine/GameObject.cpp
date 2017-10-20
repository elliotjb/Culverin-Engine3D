#include "GameObject.h"
#include "Component.h"
#include "CompMesh.h"

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

Component* GameObject::FindComponentByType(Comp_Type type)
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

Component* GameObject::AddComponent(Comp_Type type)
{
	bool dupe = false;
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type) //We need to check if the component is ACTIVE first?¿
		{
			dupe = true;
			LOG("There's already one component of this type in the Game Object.");
			break;
		}
	}

	if (!dupe)
	{
		if (type == C_MESH)
		{
			LOG("Adding MESH COMPONENT.");
			CompMesh* mesh = new CompMesh(type);
			components.push_back(mesh);

			return mesh;
		}
		
	}

	return nullptr;
}
