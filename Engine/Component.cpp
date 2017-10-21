#include "Component.h"

Component::Component(Comp_Type t):type(t)
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
