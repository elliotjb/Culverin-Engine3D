#pragma once
#include "Globals.h"


class Resource
{
public:
	enum Type {
		UNKNOWN,
		MATERIAL,
		MESH
	};

public:
	Resource(uint uid, Resource::Type type);
	virtual ~Resource();

	Resource::Type GetType() const;
	uint GetUUID() const;

protected:
	Type type = UNKNOWN;
	uint uid = 0;

public:
	char* name = "Name Resource";
	uint uuid_mesh = 0;
	int NumGameObjectsUseMe = 0;
};

