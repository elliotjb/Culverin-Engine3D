#pragma once
#include "Globals.h"
#include "Module.h"
#include "WindowProject.h"

#include <filesystem>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>

#define DIRECTORY_ASSETS "Assets\\"
#define DIRECTORY_LIBRARY "Library\\"
#define DIRECTORY_LIBRARY_MESHES "Library\\Meshes\\"
#define DIRECTORY_LIBRARY_MATERIALS "Library\\Materials\\"

enum DIRECTORY_IMPORT
{
	IMPORT_DEFAULT,
	IMPORT_DIRECTORY_ASSETS,
	IMPORT_DIRECTORY_LIBRARY,
	IMPORT_DIRECTORY_LIBRARY_MESHES,
	IMPORT_DIRECTORY_LIBRARY_MATERIALS
};

class ModuleFS : public Module
{
public:
	ModuleFS(bool start_enabled = true);
	~ModuleFS();

	bool Init(JSON_Object* node);
	bool Start();

	update_status PreUpdate(float dt);
	//update_status Update(float dt);
	//update_status PostUpdate(float dt);
	//update_status UpdateConfig(float dt);

	uint LoadFile(const char* file, char** buffer);

	//Name of file NOT DIRECTORY!!!
	bool SaveFile(const char* data, std::string name, uint size, DIRECTORY_IMPORT directory = IMPORT_DEFAULT);

	std::vector<std::string> Get_filenames(std::experimental::filesystem::path path);
	std::vector<Folders> Get_AllFolders(std::experimental::filesystem::path path);
	bool CheckIsFileExist(const std::string & name);

	std::string GetMainDirectory();

	// Utilities ----------------------------------
	void FixNames_directories(std::vector<std::string>& files);
	std::string FixName_directory(std::string files);
	std::string FixExtension(std::string file, char * newExtension);
	char* ConverttoChar(std::string name);
	void CreateFolder(const char* file_name);


	//Dont use at the moment --------- --------------
	std::string GetAssetsDirecotry();


private:
	std::vector<std::string> files;
	char ownPth[MAX_PATH];
	std::string directory_Game;
};

