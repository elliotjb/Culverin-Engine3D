#include "ModuleFS.h"
#include <filesystem>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>


ModuleFS::ModuleFS(bool start_enabled) : Module(start_enabled)
{
}


ModuleFS::~ModuleFS()
{
}

bool ModuleFS::Init(JSON_Object * node)
{
	return true;
}

bool ModuleFS::Start()
{
	return true;
}

uint ModuleFS::LoadFile(const char* file, char** buffer)
{
	std::ifstream is(file, std::ifstream::binary);
	int length = 0;
	if (is)
	{
		// get length of file:
		is.seekg(0, is.end);
		length = is.tellg();
		is.seekg(0, is.beg);

		*buffer = new char[length];

		is.read(*buffer, length);

		if (is)
		{
			LOG("File Loaded.")
		}
		else
		{
			LOG("Error %s", is.gcount());
		}
		is.close();
	}
	else
	{
		LOG("Error to Load File -> %s", file);
	}
	return length;
}


/*
std::ofstream outfile(file, std::ofstream::binary);

if (outfile.good()) {
// write to outfile
outfile.write(buffer, size);
}
else {
LOG("Failed to write the file %s", path);
}


outfile.close();
*/