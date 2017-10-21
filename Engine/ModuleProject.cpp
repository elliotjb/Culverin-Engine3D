#include "ModuleProject.h"
#include "Application.h"


Project::Project()
{
	active.push_back(Active());
	name = "Project";
}


Project::~Project()
{
	active.clear();
}

bool Project::Start()
{
	directory_see = App->fs->GetMainDirectory();
	directory_see += "\\Assets";
	Iterate_files();
	ReorderFiles();
	folders = App->fs->Get_AllFolders(directory_see);
	sizeFiles = 50;

	return true;
}

update_status Project::Update(float dt)
{
	if (active[0].active)
		ShowProject();

	return UPDATE_CONTINUE;
}

bool Project::CleanUp()
{

	return true;
}

void Project::ShowProject()
{
	if (!BeginDock("Project", NULL, 0)) //TODO ELLIOT CLOSE Windows example
	{
		EndDock();
		return;
	}
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.211f, 0.211f, 0.211f, 1.00f));
	if (ImGui::BeginChild(ImGui::GetID("Project"), ImVec2(ImGui::GetWindowWidth(), 20)))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 2));
		if (ImGui::Button("Select.."))
			ImGui::OpenPopup("select");
		ImGui::PopStyleVar(2);
		ImGui::SameLine();
		if (ImGui::BeginPopup("select"))
		{
			if (ImGui::Selectable("Create Folder..."))
			{

			}
			if (ImGui::Selectable("Import file"))
			{

			}
			ImGui::EndPopup();
		}
		// Size of Items ----------
		ImGui::SameLine(ImGui::GetWindowWidth() - 160);
		ImGui::PushItemWidth(100);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 2));
		ImGui::Text("Size:"); ImGui::SameLine();
		ImGui::SliderInt("##Size1", &sizeFiles, 25, 100);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();

	static GLuint folder_icon = App->textures->LoadTexture("Images/UI/folder_icon.png");
	static GLuint icon_png = App->textures->LoadTexture("Images/UI/icon_png.png");
	static GLuint icon_jpg = App->textures->LoadTexture("Images/UI/icon_jpg.png");
	static GLuint icon_fbx = App->textures->LoadTexture("Images/UI/icon_fbx.png");
	static GLuint icon_obj = App->textures->LoadTexture("Images/UI/icon_obj.png");
	static GLuint icon_unknown = App->textures->LoadTexture("Images/UI/icon_unknown.png");

	// ---------------------------------------------------------------------------------------------------
	ImGui::Columns(2, "Project_Assets");
	ImGui::SetColumnWidth(0, SPERATIONCOLUMN);
	//Column 1 LEFT ------------------------
	ImGui::Spacing();
	// Folders ---------------------
	Folders_update(folders);

	ImGui::NextColumn();
	ImGui::Separator();


	uint width = DISTANCEBUTTONS;
	for (int i = 0; i < files.size(); i++)
	{
		ImGui::PushID(i);

		std::string namebySize = files[i].file_name;
		ImVec2 size_name = ImGui::CalcTextSize(namebySize.c_str(), NULL, true);
		if (size_name.x > sizeFiles)
		{
			for (uint ds = namebySize.size(); ds > 0; ds--)
			{
				size_name = ImGui::CalcTextSize(namebySize.c_str(), NULL, true);
				if (size_name.x < sizeFiles)
				{
					namebySize.pop_back();
					namebySize.pop_back();
					namebySize.pop_back();
					namebySize += "...";
					break;
				}
				else
					namebySize.pop_back();
			}
		}

		switch (files[i].file_type)
		{
		case NON:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_unknown, namebySize.c_str(), ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			break;
		}
		case FOLDER:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)folder_icon, namebySize.c_str(), ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			break;
		}
		case FBX:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_fbx, namebySize.c_str(), ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			break;
		}
		case OBJ:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_obj, namebySize.c_str(), ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			break;
		}
		case PNG:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_png, namebySize.c_str(), ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			break;
		}
		case JPG:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_jpg, namebySize.c_str(), ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			break;
		}
		}
		width += sizeFiles + MARGEBUTTON + DISTANCEBUTTONS; // Size of imatge + marge button + between buttons
		int temp = ImGui::GetWindowWidth();
		if ((width + DISTANCEBUTTONS + SPERATIONCOLUMN) > temp)
		{
			width = 0;
		}
		else
		{
			if(i + 1 <  files.size())
				ImGui::SameLine();
		}


		ImGui::PopID();
	}

	EndDock();
}

void Project::Iterate_files()
{
	std::vector<std::string> temp_directory = App->fs->Get_filenames(directory_see);
	std::vector<std::string> temp_name = temp_directory;
	App->fs->FixNames_directories(temp_name);
	for (int i = 0; i < temp_name.size(); i++)
	{
		Folder_Files file_temp;
		file_temp.directory_name = temp_directory[i];
		char* temp = new char[temp_name[i].length() + 1];
		strcpy(temp, temp_name[i].c_str());
		temp[temp_name[i].length()] = '\0';
		file_temp.file_name = temp;
		file_temp.file_type = SetType(temp_name[i]);
		files.push_back(file_temp);
	}
}

TYPE_FILE Project::SetType(std::string name)
{
	size_t EndName = name.find_last_of(".");
	std::string temp = name.substr(EndName + 1);
	if (name.compare(temp) != 0)
	{
		// Isn't a Folder
		if (temp == "png")
		{
			return PNG;
		}
		else if (temp == "jpg")
		{
			return JPG;
		}
		else if (temp == "dds")
		{
			return DDS;
		}
		else if (temp == "fbx")
		{
			return FBX;
		}
		else if (temp == "obj")
		{
			return OBJ;
		}
		else
		{
			return NON;
		}
	}
	else
	{
		return FOLDER;
	}
}

void Project::ReorderFiles()
{
	uint size = 0;
	for (uint i = 0; i < files.size(); i++)
	{
		if (files[i].file_type == FOLDER)
		{
			for (int j = 0; j < files.size(); j++)
			{
				if (j == size)
				{
					Swap(files[i], files[j]);
					size++;
					break;
				}
			}
		}
	}
}

void Project::Folders_update(std::vector<Folders> folder)
{
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.335f, 0.337f, 0.357f, 1.00f));
	for (int i = 0; i < folder.size(); i++)
	{
		ImGui::PushID(i);
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		if (ImGui::TreeNodeEx(folder[i].file_name, node_flags))
		{
			if (ImGui::BeginPopupContextItem("rename context menu"))
			{
				ImGui::Text("Edit name:");
				ImGui::InputText("##edit2", folder[i].file_name, 256);
				ImGui::EndPopup();
			}
			if (ImGui::IsItemClicked())
				folder[i].active = !folder[i].active;

			if (folder[i].folder_child.size() > 0)
			{
				Folders_update(folder[i].folder_child);
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	ImGui::PopStyleColor();
}
