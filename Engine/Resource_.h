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
	enum State {
		UNLOADED,
		LOADED,
		REIMPORTED,
		WANTDELETE
	};

public:
	Resource(uint uuid, Resource::Type type, Resource::State state);
	virtual ~Resource();

	Resource::Type GetType() const;
	Resource::State GetState() const;
	uint GetUUID() const;
	void SetState(Resource::State state);

	virtual void DeleteToMemory(){}
	virtual Resource::State IsLoadedToMemory()
	{
		return state;
	}

protected:
	Type type = Type::UNKNOWN;
	State state = State::UNLOADED;
	uint uuid = 0;

public:
	char* name = "Name Resource";
	uint NumGameObjectsUseMe = 0;
};

