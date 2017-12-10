#ifndef _SCRIPTING_
#define _SCRIPTING_

#include "Module.h"

class ModuleScripting: public Module
{
public:
	ModuleScripting(bool start_enabled = true);
	~ModuleScripting();

	bool Init(JSON_Object* node);
	update_status Update(float dt);
	bool CleanUp();

private:

};

#endif
