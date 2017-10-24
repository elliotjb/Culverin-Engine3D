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

struct Folder_Files
{
	std::string directory_name;
	char* file_name = nullptr;
	TYPE_FILE file_type;
};

struct Folders
{
	std::string directory_name;
	char* file_name = nullptr;
	std::vector<Folders> folder_child;
	bool active = false;
	Folder_Files* files = nullptr; // Pointer to files that you see now.
};

class Project : public WindowManager
{
public:

	Project();
	virtual ~Project();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ShowProject();
	void Iterate_files();
	TYPE_FILE SetType(std::string name);
	void ReorderFiles();
	void Folders_update(const std::vector<Folders>& folder);


private:
	std::vector<Folder_Files> files;
	std::vector<Folders> folders;
	std::string directory_see;
	int sizeFiles;
};
