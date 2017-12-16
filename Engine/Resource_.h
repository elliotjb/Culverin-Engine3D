#ifndef _RESOURCE_
#define _RESOURCE_

#include "Globals.h"
#include <string>

class Resource
{
public:
	enum Type {
		UNKNOWN,
		FOLDER,
		MATERIAL,
		MESH,
		SCRIPT
	};
	enum State {
		UNLOADED,
		LOADED,
		REIMPORTED,
		WANTDELETE,
		FAILED
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
	std::string path_assets;
	uint NumGameObjectsUseMe = 0;
};

#endif