#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui.h"
#include "WindowManager.h"

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
	std::string directory_name;
	std::string directory_name_next;
	char* file_name = nullptr;
	TYPE_FILE file_type;
};

struct FoldersNew
{
	std::string directory_name;
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

	//void ShowProject();
	//void Iterate_files(std::vector<Folders>& folders);
	TYPE_FILE SetType(std::string name);
	//void ReorderFiles(std::vector<Folders>& folders);
	//void Files_Update(const std::vector<Files>& files);
	//const std::vector<Files>& Folders_update(std::vector<Folders>& folder);

	//NEW 
	void ShowProjectNew();
	void Folders_updateNew(std::vector<FoldersNew> folders);

	void Files_UpdateNew(const std::vector<FilesNew>& files);
	void DeleteFiles(std::vector<FilesNew> files);
	void DeleteFolders(std::vector<FoldersNew> folders);
	void SetAllFolderBoolNew(std::vector<FoldersNew>& folders, bool setBoolean);

	//void SetAllFolderBool(std::vector<Folders>& folders, bool setBoolean);
	//void ChangefileViwer(std::vector<Folders>& folder, std::string name);

	std::string GetFolderSee();
	std::vector<Files>* GetFilesSee();
	//void AddFile(std::vector<Files>* folderViwe, std::string newFile);

private:

	//std::vector<Folders> folders;
	std::string directory_see;
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
};
