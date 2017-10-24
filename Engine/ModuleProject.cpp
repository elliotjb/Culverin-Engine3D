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
	//directory_see = App->fs->GetMainDirectory();
	//directory_see = "Assets\\";
	directory_see = "";
	folders = App->fs->Get_AllFolders(directory_see);
	Iterate_files(folders);
	ReorderFiles(folders);
	folders[0].active = true;
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

	// ---------------------------------------------------------------------------------------------------
	ImGui::Columns(2, "Project_Assets");
	ImGui::SetColumnWidth(0, SPERATIONCOLUMN);
	//Column 1 LEFT ------------------------
	ImGui::Spacing();
	// Folders ---------------------
	std::vector<Files>* filesAvtive = Folders_update(folders);

	ImGui::NextColumn();
	ImGui::Separator();
	Files_Update(*filesAvtive);

	EndDock();
}

void Project::Iterate_files(std::vector<Folders>& folders)
{
	for (int i = 0; i < folders.size(); i++)
	{
		if (folders[i].folder_child.size() > 0)
		{
			//for (int x = 0; x < folders[i].folder_child.size(); x++)
			//{
			//	folders[i].folder_child[x].parent = &folders[i];
			//}
			Iterate_files(folders[i].folder_child);
		}
		std::vector<std::string> temp_directory = App->fs->Get_filenames(folders[i].directory_name);
		std::vector<std::string> temp_name = temp_directory;
		App->fs->FixNames_directories(temp_name);
		for (int j = 0; j < temp_name.size(); j++)
		{
			Files file_temp;
			file_temp.directory_name = temp_directory[j];
			char* temp = new char[temp_name[j].length() + 1];
			strcpy(temp, temp_name[j].c_str());
			temp[temp_name[j].length()] = '\0';
			file_temp.file_name = temp;
			file_temp.file_type = SetType(temp_name[j]);
			file_temp.parentFolder = &folders[i];
			folders[i].files.push_back(file_temp);
		}
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

void Project::ReorderFiles(std::vector<Folders>& folders)
{
	uint size = 0;
	for (uint i = 0; i < folders.size(); i++)
	{
		if (folders[i].folder_child.size() > 0)
		{
			ReorderFiles(folders[i].folder_child);
		}
		for (int x = 0; x < folders[i].files.size(); x++)
		{
			if (folders[i].files[x].file_type == FOLDER)
			{
				for (int j = 0; j < folders[i].files.size(); j++)
				{
					if (j == size && folders[i].files[j].file_type != FOLDER)
					{
						Swap(folders[i].files[x], folders[i].files[j]);
						size++;
						break;
					}
				}
			}
		}
	}
}

void Project::Files_Update(std::vector<Files>& files)
{
	static GLuint folder_icon = App->textures->LoadTexture("Images/UI/folder_icon.png");
	static GLuint icon_png = App->textures->LoadTexture("Images/UI/icon_png.png");
	static GLuint icon_jpg = App->textures->LoadTexture("Images/UI/icon_jpg.png");
	static GLuint icon_fbx = App->textures->LoadTexture("Images/UI/icon_fbx.png");
	static GLuint icon_obj = App->textures->LoadTexture("Images/UI/icon_obj.png");
	static GLuint icon_unknown = App->textures->LoadTexture("Images/UI/icon_unknown.png");
	uint width = DISTANCEBUTTONS;
	for (int i = 0; i < files.size(); i++)
	{
		ImGui::PushID(i);

		//std::string namebySize = files[i].file_name;
		static char nameTemp[100];
		int lenght = strlen(files[i].file_name);
		memset(nameTemp, 0, 99);
		for (int j = 0; j < lenght; j++)
		{
			nameTemp[j] = files[i].file_name[j];
		}
		//ImVec2 size_name = ImGui::CalcTextSize(namebySize.c_str(), NULL, true);
		ImVec2 size_name = ImGui::CalcTextSize(nameTemp, NULL, true);
		if (size_name.x > sizeFiles)
		{
			int siz = strlen(nameTemp);
			for (uint ds = siz; ds > 0; ds--)
			{
				uint st = strlen(nameTemp);
				size_name = ImGui::CalcTextSize(nameTemp, NULL, true);
				if (size_name.x < sizeFiles)
				{
					nameTemp[st - 3] = '.';
					nameTemp[st - 2] = '.';
					nameTemp[st - 1] = '.';
					nameTemp[st] = '\0';
					break;
				}
				else
					nameTemp[st - 1] = 0;
			}
		}

		switch (files[i].file_type)
		{
		case NON:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_unknown, nameTemp, ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));

			break;
		}
		case FOLDER:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)folder_icon, nameTemp, ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
			{
				//LOG("%.2f - %.2f  / /  %.2f - %.2f", ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y, ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y);
				if (ImGui::IsMouseDoubleClicked(0))
				{
					ChangefileViwer(files[i].parentFolder->folder_child, files[i].file_name);
				}
			}
			break;
		}
		case FBX:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_fbx, nameTemp, ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			break;
		}
		case OBJ:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_obj, nameTemp, ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			break;
		}
		case PNG:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_png, nameTemp, ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
			break;
		}
		case JPG:
		{
			ImGui::ImageButtonWithTextDOWN_NoReajust((ImTextureID*)icon_jpg, nameTemp, ImVec2(sizeFiles, sizeFiles), ImVec2(-1, 1), ImVec2(0, 0));
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
			if (i + 1 <  files.size())
				ImGui::SameLine();
		}


		ImGui::PopID();
	}

}

std::vector<Files>* Project::Folders_update(std::vector<Folders>& folder)
{

	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.335f, 0.337f, 0.357f, 1.00f));
	for (int i = 0; i < folder.size(); i++)
	{
		ImGui::PushID(i);
		ImGuiTreeNodeFlags node_flags;
		if (folder[i].active)
		{
			node_flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow;
		}
		else
		{
			node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
		}
		if (ImGui::TreeNodeEx(folder[i].file_name, node_flags))
		{
			if (ImGui::BeginPopupContextItem("rename context menu"))
			{
				ImGui::Text("Edit name:");
				ImGui::InputText("##edit2", folder[i].file_name, 256);
				ImGui::EndPopup();
			}
			if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
			{
				//LOG("%.2f - %.2f  / /  %.2f - %.2f", ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y, ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y);
				if (ImGui::IsMouseDoubleClicked(0) && folder[i].active == false)
				{
					SetAllFolderBool(folders, false);
					folder[i].active = true;
					fileViwer = &folder[i].files;
				}
			}
			if (folder[i].folder_child.size() > 0)
			{
				Folders_update(folder[i].folder_child);
			}
			ImGui::TreePop();
		}
		//if (folder[i].active)
		//{
		//	
		//}
		ImGui::PopID();
	}
	ImGui::PopStyleColor();
	if (fileViwer == nullptr)
	{
		fileViwer = &folder[0].files;
	}
	return fileViwer;
}

void Project::SetAllFolderBool(std::vector<Folders>& folders, bool setBoolean)
{
	for (int i = 0; i < folders.size(); i++)
	{
		if (folders[i].folder_child.size() > 0)
		{
			SetAllFolderBool(folders[i].folder_child, setBoolean);
		}
		folders[i].active = setBoolean;
	}
}

void Project::ChangefileViwer(std::vector<Folders>& folder, std::string name)
{
	SetAllFolderBool(folders, false);
	for (int i = 0; i < folder.size(); i++)
	{
		if (folder[i].file_name == name)
		{
			folder[i].active = true;
			fileViwer = &folder[i].files;
		}
	}	
}
