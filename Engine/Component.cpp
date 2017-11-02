#include "Component.h"
#include "GameObject.h"

Component::Component(Comp_Type t, GameObject* parent) :type(t), parent(parent)
{
}

Component::~Component()
{
}

bool Component::Enable()
{
	if (!active)
	{
		active = true;
	}
	return active;
}

bool Component::Disable()
{
	if (active)
	{
		active = false;
	}
	return active;
}

void Component::preUpdate()
{
}

void Component::Update()
{
}

void Component::ShowInspectorInfo()
{
}

Comp_Type Component::GetType() const
{
	return type;
}

bool Component::isActive() const
{
	return active;
}

void Component::Save(JSON_Object * object, std::string name) const
{
}

void Component::Load(const JSON_Object * object, std::string name)
{
}