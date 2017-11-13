#include "ModuleFS.h"
#include "Application.h"

ModuleFS::ModuleFS(bool start_enabled) : Module(start_enabled)
{
	Start_enabled = true;
	preUpdate_enabled = true;

	name = "File System";
}


ModuleFS::~ModuleFS()
{
}

//bool ModuleFS::Init(JSON_Object * node)
//{
//	perf_timer.Start();
//
//	Awake_t = perf_timer.ReadMs();
//	return true;
//}

bool ModuleFS::Start()
{
	perf_timer.Start();

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
	directory_Game += "\\Game"; // "\\Game\\Assets"

	// Check if Main Folders exist --------------------
	CreateFolder("Library");
	CreateFolder("Library\\Meshes");
	CreateFolder("Library\\Materials");
	CreateFolder("Library\\Animations");
	CreateFolder("Assets");
	//Iterate All Game
	//files = Get_filenames("Assets\\");

	Start_t = perf_timer.ReadMs();
	return true;
}

update_status ModuleFS::PreUpdate(float dt)
{
	perf_timer.Start();

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		//char* buffer = nullptr;

		//// Loading File
		//uint size = LoadFile(files[1].c_str(), &buffer);
		//int x = 0;

		//CopyFileToAssets("C:\\Users\\elliotjb\\Desktop\\street\\Street environment_V01.FBX", "C:\\Users\\elliotjb\\Desktop\\3D-Engine\\Street environment_V01.FBX");

		//uint temp = App->random->Int();
		//LOG("%i", temp);
	}

	preUpdate_t = perf_timer.ReadMs();
	return UPDATE_CONTINUE;
}

//update_status ModuleFS::Update(float dt)
//{
//	perf_timer.Start();
//
//	Update_t = perf_timer.ReadMs();
//	return UPDATE_CONTINUE;
//}

//update_status ModuleFS::PostUpdate(float dt)
//{
//	perf_timer.Start();
//
//	postUpdate_t = perf_timer.ReadMs();
//	return UPDATE_CONTINUE;
//}

void ModuleFS::CopyFileToAssets(const char* fileNameFrom, const char* fileNameTo)
{
	//assert(fileExists(fileNameFrom));
	namespace fs = std::experimental::filesystem;
	std::string exits = fileNameTo;
	exits += "/" + FixName_directory(fileNameFrom);
	if (fs::exists(exits) == false)
	{
		fs::copy(fileNameFrom, fileNameTo);
	}
	exits.clear();
	// Copy Folders
	//std::filesystem::copy("/dir1", "/dir3", std::filesystem::copy_options::recursive);
}

bool ModuleFS::CheckAssetsIsModify()
{
	return true;
}

void ModuleFS::GetAllFolders(std::experimental::filesystem::path path, std::string folderActive, std::vector<FoldersNew>& folders)
{
	namespace stdfs = std::experimental::filesystem;

	DeleteFolders(folders);

	const stdfs::directory_iterator end{};

	if (path == "")
	{
		path = directory_Game;
	}

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		if (stdfs::is_directory(*iter))
		{
			FoldersNew folder_temp;

			folder_temp.directory_name = ConverttoConstChar(iter->path().string());
			folder_temp.file_name = ConverttoChar(FixName_directory(iter->path().string()));
			if (folderActive == folder_temp.directory_name)
			{
				folder_temp.active = true;
			}
			GetAllFoldersChild(iter->path().string(), folderActive, folder_temp.folder_child);
			folders.push_back(folder_temp);
		}
	}
}

void ModuleFS::GetAllFoldersChild(std::experimental::filesystem::path path, std::string folderActive, std::vector<FoldersNew>& folders)
{
	namespace stdfs = std::experimental::filesystem;

	const stdfs::directory_iterator end{};

	if (path == "")
	{
		path = directory_Game;
	}

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		if (stdfs::is_directory(*iter))
		{
			FoldersNew folder_temp;
			folder_temp.directory_name = ConverttoConstChar(iter->path().string());
			folder_temp.file_name = ConverttoChar(FixName_directory(iter->path().string()));
			if (folderActive == folder_temp.directory_name)
			{
				folder_temp.active = true;
			}
			GetAllFoldersChild(iter->path().string(), folderActive, folder_temp.folder_child);
			folders.push_back(folder_temp);
		}
	}
}

void ModuleFS::GetAllFiles(std::experimental::filesystem::path path, std::vector<FilesNew>& files)
{
	namespace stdfs = std::experimental::filesystem;

	DeleteFiles(files);

	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		FilesNew files_temp;
		files_temp.directory_name = ConverttoConstChar(iter->path().string());
		if (stdfs::is_directory(*iter))
		{
			files_temp.directory_name_next = ConverttoConstChar(iter->path().string() + "\\");
		}
		else
			files_temp.directory_name_next = nullptr;

		files_temp.file_name = ConverttoChar(FixName_directory(iter->path().string()));
		files_temp.file_type = ((Project*)App->gui->winManager[PROJECT])->SetType(files_temp.file_name);
		files.push_back(files_temp);
	}
}

void ModuleFS::DeleteFiles(std::vector<FilesNew>& files)
{
	for (int i = 0; i < files.size(); i++)
	{
		RELEASE_ARRAY(files[i].directory_name);
		RELEASE_ARRAY(files[i].directory_name_next)
		RELEASE_ARRAY(files[i].file_name)
	}
	files.clear();
}

void ModuleFS::DeleteFolders(std::vector<FoldersNew>& folders)
{
	for (int i = 0; i < folders.size(); i++)
	{
		RELEASE_ARRAY(folders[i].directory_name);
		RELEASE_ARRAY(folders[i].file_name);
		DeleteFolders(folders[i].folder_child);
	}
	folders.clear();
}

uint ModuleFS::LoadFile(const char* file, char** buffer, DIRECTORY_IMPORT directory)
{
	// First apply direcotry
	std::string temp = file;
	switch (directory)
	{
	case IMPORT_DEFAULT:
	{
		break;
	}
	case IMPORT_DIRECTORY_ASSETS:
	{
		temp = DIRECTORY_ASSETS + temp;
		break;
	}
	case IMPORT_DIRECTORY_LIBRARY:
	{
		temp = DIRECTORY_LIBRARY + temp;
		break;
	}
	case IMPORT_DIRECTORY_LIBRARY_MESHES:
	{
		temp = DIRECTORY_LIBRARY_MESHES + temp;
		break;
	}
	case IMPORT_DIRECTORY_LIBRARY_MATERIALS:
	{
		temp = DIRECTORY_LIBRARY_MATERIALS + temp;
		break;
	}
	}
	std::ifstream is(temp, std::ifstream::binary);
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

// If newExtension == nullptr, only return name not extension
std::string ModuleFS::FixExtension(std::string file, const char* newExtension)
{
	size_t EndName = file.find_last_of(".");
	file = file.substr(0, EndName);
	if(newExtension != nullptr)
		file += newExtension;
	return file;
}

const char* ModuleFS::GetExtension(std::string file)
{
	size_t EndName = file.find_last_of(".");
	file = file.substr(EndName, 0);
	return file.c_str();
}

char* ModuleFS::ConverttoChar(std::string name)
{
	char* temp = new char[name.length() + 1];
	strcpy(temp, name.c_str());
	temp[name.length()] = '\0';
	return temp;
}

const char* ModuleFS::ConverttoConstChar(std::string name)
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

std::string ModuleFS::AddDirectorybyType(std::string name, DIRECTORY_IMPORT directory)
{
	std::string temp = name;
	switch (directory)
	{
	case IMPORT_DEFAULT:
	{
		break;
	}
	case IMPORT_DIRECTORY_ASSETS:
	{
		temp = DIRECTORY_ASSETS + temp;
		break;
	}
	case IMPORT_DIRECTORY_LIBRARY:
	{
		temp = DIRECTORY_LIBRARY + temp;
		break;
	}
	case IMPORT_DIRECTORY_LIBRARY_MESHES:
	{
		temp = DIRECTORY_LIBRARY_MESHES + temp;
		break;
	}
	case IMPORT_DIRECTORY_LIBRARY_MATERIALS:
	{
		temp = DIRECTORY_LIBRARY_MATERIALS + temp;
		break;
	}
	}
	return temp;
}

void ModuleFS::CreateFolder(const char* file_name)
{
	namespace fs = std::experimental::filesystem;

	if (!fs::exists(file_name)) { // Check if src folder exists
		fs::create_directory(file_name); // create src folder
	}
}

// Special JSON Array Float3
JSON_Status ModuleFS::json_array_dotset_float3(JSON_Object *object, std::string name, float3 transform)
{
	JSON_Value* value = json_value_init_array();
	if (value == NULL) {
		return JSONFailure;
	}
	JSON_Array* arry = json_value_get_array(value);
	if (json_object_dotset_value(object, name.c_str(), value) == JSONFailure)
	{
		json_value_free(value);
		return JSONFailure;
	}
	json_array_append_number(arry, transform.x);
	json_array_append_number(arry, transform.y);
	json_array_append_number(arry, transform.z);
	return JSONSuccess;
}

JSON_Status ModuleFS::json_array_dotset_float2(JSON_Object *object, std::string name, float2 transform)
{
	JSON_Value* value = json_value_init_array();
	if (value == NULL) {
		return JSONFailure;
	}
	JSON_Array* arry = json_value_get_array(value);
	if (json_object_dotset_value(object, name.c_str(), value) == JSONFailure)
	{
		json_value_free(value);
		return JSONFailure;
	}
	json_array_append_number(arry, transform.x);
	json_array_append_number(arry, transform.y);
	return JSONSuccess;
}

JSON_Status ModuleFS::json_array_dotset_float4(JSON_Object *object, std::string name, float4 transform)
{
	JSON_Value* value = json_value_init_array();
	if (value == NULL) {
		return JSONFailure;
	}
	JSON_Array* arry = json_value_get_array(value);
	if (json_object_dotset_value(object, name.c_str(), value) == JSONFailure)
	{
		json_value_free(value);
		return JSONFailure;
	}
	json_array_append_number(arry, transform.x);
	json_array_append_number(arry, transform.y);
	json_array_append_number(arry, transform.z);
	json_array_append_number(arry, transform.w);
	return JSONSuccess;
}

float3 ModuleFS::json_array_dotget_float3_string(const JSON_Object* object, std::string name)
{
	JSON_Array* array = json_object_dotget_array(object, name.c_str());
	float3 transform;
	transform.x = (float)json_value_get_number(json_array_get_value(array, 0));
	transform.y = (float)json_value_get_number(json_array_get_value(array, 1));
	transform.z = (float)json_value_get_number(json_array_get_value(array, 2));

	return transform;
}
float2 ModuleFS::json_array_dotget_float2_string(const JSON_Object* object, std::string name)
{
	JSON_Array* array = json_object_dotget_array(object, name.c_str());
	float2 transform;
	transform.x = (float)json_value_get_number(json_array_get_value(array, 0));
	transform.y = (float)json_value_get_number(json_array_get_value(array, 1));

	return transform;
}
float4 ModuleFS::json_array_dotget_float4_string(const JSON_Object* object, std::string name)
{
	JSON_Array* array = json_object_dotget_array(object, name.c_str());
	float4 transform;
	transform.x = (float)json_value_get_number(json_array_get_value(array, 0));
	transform.y = (float)json_value_get_number(json_array_get_value(array, 1));
	transform.z = (float)json_value_get_number(json_array_get_value(array, 2));
	transform.w = (float)json_value_get_number(json_array_get_value(array, 3));

	return transform;
}