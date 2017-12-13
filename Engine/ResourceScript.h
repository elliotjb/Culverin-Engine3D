#ifndef _RESOURCESCRIPT_
#define _RESOURCESCRIPT_

#include "Resource_.h"
#include "Globals.h"

class ResourceScript : public Resource
{
public:
	ResourceScript(uint uid);
	virtual ~ResourceScript();

	void DeleteToMemory();
	bool LoadToMemory();
	Resource::State IsLoadedToMemory();

private:
	// 

};


#endif