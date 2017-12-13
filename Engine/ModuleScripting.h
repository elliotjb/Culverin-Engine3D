#ifndef _SCRIPTING_
#define _SCRIPTING_

#include "Module.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

class ModuleScripting: public Module
{
public:
	ModuleScripting(bool start_enabled = true);
	~ModuleScripting();

	bool Init(JSON_Object* node);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
	void AddInternalCalls();

};

#endif
