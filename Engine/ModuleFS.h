#pragma once
#include "Globals.h"
#include "Module.h"
#include "WindowProject.h"
#include "Timer.h"

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

struct AllFiles
{
	const char* directory_name = nullptr;
	char* file_name = nullptr;
	std::time_t ftime;
};

class ModuleFS : public Module
{
public:
	ModuleFS(bool start_enabled = true);
	~ModuleFS();

	bool Start();
	update_status PreUpdate(float dt);

	void CopyFileToAssets(const char * fileNameFrom, const char * fileNameTo);

	//update_status UpdateConfig(float dt);

	uint LoadFile(const char* file, char** buffer, DIRECTORY_IMPORT directory = IMPORT_DEFAULT);

	//Name of file NOT DIRECTORY!!!
	bool SaveFile(const char* data, std::string name, uint size, DIRECTORY_IMPORT directory = IMPORT_DEFAULT);

	// Get Files to Window Project ---------------------------------------------------
	void GetAllFolders(std::experimental::filesystem::path path, std::string folderActive, std::vector<FoldersNew>& folders);
	void GetAllFoldersChild(std::experimental::filesystem::path path, std::string folderActive, std::vector<FoldersNew>& folders);
	void GetAllFiles(std::experimental::filesystem::path path, std::vector<FilesNew>& files);

	// Get Files to check if a file was modificated ---------------------------------
	void GetAllFilesAssets(std::experimental::filesystem::path path, std::vector<AllFiles>& files);
	bool AnyfileModificated(std::vector<AllFiles>& files);
	void UpdateFilesAsstes();
	void CheckNowfilesAssets();


	void DeleteFiles(std::vector<FilesNew>& files);
	void DeleteFolders(std::vector<FoldersNew>& folders);
	void DeleteAllFilesAssets(std::vector<AllFiles>& filesAssets);

	bool CheckIsFileExist(const std::string & name);

	std::string GetMainDirectory();

	// Utilities ----------------------------------
	void FixNames_directories(std::vector<std::string>& files);
	std::string FixName_directory(std::string files);
	std::string FixExtension(std::string file, const char* newExtension = nullptr);
	std::string GetExtension(std::string file);
	char* ConverttoChar(std::string name);
	const char* ConverttoConstChar(std::string name);
	void CreateFolder(const char* file_name);

	// 
	bool CheckAssetsIsModify();

	// Functions Serialitzation 
	// Special JSON Array -> float3, float2, Color
	// float3
	JSON_Status json_array_dotset_float3(JSON_Object *object, std::string name, float3 transform);
	// float2
	JSON_Status json_array_dotset_float2(JSON_Object *object, std::string name, float2 transform);
	// Color - (r,g,b,w) or (x,y,z,w).
	JSON_Status json_array_dotset_float4(JSON_Object *object, std::string name, float4 transform);

	float3 json_array_dotget_float3_string(const JSON_Object* object, std::string name);
	float2 json_array_dotget_float2_string(const JSON_Object* object, std::string name);
	float4 json_array_dotget_float4_string(const JSON_Object* object, std::string name);


	//Dont use at the moment --------- --------------
	std::string GetAssetsDirecotry();

	std::string AddDirectorybyType(std::string name, DIRECTORY_IMPORT directory);


private:
	Timer checkAssets;
	std::vector<AllFiles> allfilesAsstes;

	char ownPth[MAX_PATH];
	std::string directory_Game;
	std::string directory_Assets;
	//std::vector<FoldersNew> filenames;
};

