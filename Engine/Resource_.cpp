#include "Resource_.h"


Resource::Resource(uint uid, Resource::Type type) : uid(uid), type(type)
{
}

Resource::~Resource()
{
}

Resource::Type Resource::GetType() const
{
	return type;
}

uint Resource::GetUUID() const
{
	return uid;
}
