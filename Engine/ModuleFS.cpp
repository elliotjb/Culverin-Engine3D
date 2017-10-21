#include "ModuleFS.h"
#include "Application.h"

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
	// Will contain exe path
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// When passing NULL to GetModuleHandle, it returns handle of exe itself
		GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
	}
	return true;
}

update_status ModuleFS::PreUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		files = Get_filenames(ownPth);
	}


	return UPDATE_CONTINUE;
}

std::vector<std::string> ModuleFS::Get_filenames(std::experimental::filesystem::path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> filenames;

	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		if (stdfs::is_regular_file(*iter)) // comment out if all names (names of directories tc.) are required
			filenames.push_back(iter->path().string());
	}

	return filenames;
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

bool ModuleFS::SaveFile(const char* data, std::string name, uint size)
{
	//Move to FileSystem ----------------------------------------
	std::ofstream outfile(name, std::ofstream::binary);

	if (outfile.good())
	{
		// write to outfile
		outfile.write(data, size);
		LOG("Save Completed");
	}
	else
	{
		LOG("Failed to write the file %s", name.c_str());
	}

	outfile.close();
	// ------------------------------------------------------------
	return false;
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