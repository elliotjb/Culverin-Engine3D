#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"
#include "Timer.h"

#define MARGEBUTTON 20
#define DISTANCEBUTTONS 10
#define SPERATIONCOLUMN 200

enum TYPE_FILE
{
	NON,
	FOLDER,
	FBX,
	OBJ,
	PNG,
	JPG,
	DDS
};
struct Folders;

struct Files
{
	std::string directory_name;
	char* file_name = nullptr;
	TYPE_FILE file_type;
	Folders* parentFolder = nullptr;
};

struct Folders
{
	std::string directory_name;
	char* file_name = nullptr;
	std::vector<Folders> folder_child;
	std::vector<Files> files;
	bool active = false;
};


struct FilesNew
{
	const char* directory_name = nullptr;
	const char* directory_name_next = nullptr;
	char* file_name = nullptr;
	TYPE_FILE file_type;
};

struct FoldersNew
{
	const char* directory_name = nullptr;
	bool active = false;
	char* file_name = nullptr;
	std::vector<FoldersNew> folder_child;
};

class Project : public WindowManager
{
public:

	Project();
	virtual ~Project();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	TYPE_FILE SetType(std::string name);

	//NEW 
	void ShowProject();
	void Folders_update(std::vector<FoldersNew> folders);

	void Files_Update(const std::vector<FilesNew>& files);
	void DeleteFiles(std::vector<FilesNew> files);
	void DeleteFolders(std::vector<FoldersNew> folders);

	void SetAllFolderBool(std::vector<FoldersNew>& folders, bool setBoolean);
	std::string GetFolderSee();
	std::vector<Files>* GetFilesSee();
	//void AddFile(std::vector<Files>* folderViwe, std::string newFile);

private:

	//std::vector<Folders> folders;
	char* directory_see = nullptr;
	int sizeFiles;
	std::vector<Files>* fileViwer = nullptr;

	std::vector<FoldersNew> folders;
	std::vector<FilesNew> files;

	uint folder_icon;
	uint icon_png;
	uint icon_jpg;
	uint icon_fbx;
	uint icon_obj;
	uint icon_unknown;
	Timer timeFolders;
	Timer timeFiles;
};
