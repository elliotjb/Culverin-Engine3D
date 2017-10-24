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
	directory_Game = ownPth;

	//In release this code: -----------------------------------------
	//size_t EndName = directory_Game.find_last_of("\\");
	//directory_Game = directory_Game.substr(0, EndName);

	//Not release this: ---------------------------------------------
	size_t EndName = directory_Game.find_last_of("\\");
	directory_Game = directory_Game.substr(0, EndName);
	EndName = directory_Game.find_last_of("\\");
	directory_Game = directory_Game.substr(0, EndName);
	directory_Game += "\\Game";

	// Check if Main Folders exist --------------------
	CreateFolder("Library");
	CreateFolder("Library\\Meshes");
	CreateFolder("Library\\Materials");
	CreateFolder("Library\\Animations");
	CreateFolder("Assets");
	//Iterate All Game
	files = Get_filenames("Assets\\");
	return true;
}

update_status ModuleFS::PreUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		char* buffer = nullptr;

		// Loading File
		uint size = LoadFile(files[1].c_str(), &buffer);
		int x = 0;
	}


	return UPDATE_CONTINUE;
}

std::vector<std::string> ModuleFS::Get_filenames(std::experimental::filesystem::path path)
{
	
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> filenames;

	const stdfs::directory_iterator end{};//End iterator

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		//if (stdfs::is_regular_file(*iter))
			filenames.push_back(iter->path().string());
	}

	return filenames;
}

std::vector<Folders> ModuleFS::Get_AllFolders(std::experimental::filesystem::path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<Folders> filenames;

	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		if (stdfs::is_directory(*iter))
		{
			Folders folder_temp;
			folder_temp.directory_name = iter->path().string();
			folder_temp.file_name = ConverttoChar(FixName_directory(iter->path().string()));
			folder_temp.folder_child = Get_AllFolders(iter->path().string());
			filenames.push_back(folder_temp);
		}
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

bool ModuleFS::SaveFile(const char* data, std::string name, uint size, DIRECTORY_IMPORT directory)
{
	// First apply direcotry
	switch (directory)
	{
	case IMPORT_DEFAULT:
	{
		break;
	}
	case IMPORT_DIRECTORY_ASSETS:
	{
		name = DIRECTORY_ASSETS + name;
		break;
	}
	case IMPORT_DIRECTORY_LIBRARY:
	{
		name = DIRECTORY_LIBRARY + name;
		break;
	}
	case IMPORT_DIRECTORY_LIBRARY_MESHES:
	{
		name = DIRECTORY_LIBRARY_MESHES + name;
		break;
	}
	case IMPORT_DIRECTORY_LIBRARY_MATERIALS:
	{
		name = DIRECTORY_LIBRARY_MATERIALS + name;
		break;
	}
	}
	// Open or Created ----------------------------------------
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

bool ModuleFS::CheckIsFileExist(const std::string& name) {
	std::ifstream fil(name.c_str());
	return fil.good();
}

std::string ModuleFS::GetMainDirectory()
{
	return directory_Game;
}

void ModuleFS::FixNames_directories(std::vector<std::string>& files)
{
	for (int i = 0; i < files.size(); i++)
	{
		size_t EndName = files[i].find_last_of("\\");
		files[i] = files[i].substr(EndName + 1);
	}
}

std::string ModuleFS::FixName_directory(std::string file)
{
	size_t EndName = file.find_last_of("\\");
	file = file.substr(EndName + 1);
	return file;
}

std::string ModuleFS::FixExtension(std::string file, char* newExtension)
{
	size_t EndName = file.find_last_of(".");
	file = file.substr(0, EndName);
	file += newExtension;
	return file;
}

char* ModuleFS::ConverttoChar(std::string name)
{
	char* temp = new char[name.length() + 1];
	strcpy(temp, name.c_str());
	temp[name.length()] = '\0';
	return temp;
}

std::string ModuleFS::GetAssetsDirecotry()
{
	return std::string();
}

void ModuleFS::CreateFolder(const char* file_name)
{
	namespace fs = std::experimental::filesystem;

	if (!fs::exists(file_name)) { // Check if src folder exists
		fs::create_directory(file_name); // create src folder
	}
}