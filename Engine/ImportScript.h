#ifndef _IMPORTSCRIPT_
#define _IMPORTSCRIPT_

#include "Globals.h"

class ImportScript
{
public:
	ImportScript();
	~ImportScript();

	bool Import(const char* file, uint uuid = 0);
};

#endif