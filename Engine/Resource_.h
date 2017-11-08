#pragma once
#include "Globals.h"

class Resource
{
public:
	enum Type {
		UNKNOWN,
		MATERIAL,
		MESH,
		PREFAB,
	};

public:
	Resource(uint uid, Resource::Type type);
	virtual ~Resource();

	Resource::Type GetType() const;
	uint GetUUID() const;


public:
	Type type = UNKNOWN;	uint uid = 0;
};

