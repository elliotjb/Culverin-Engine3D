#include "WindowProject.h"
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
	folder_icon = App->textures->LoadTexture("Images/UI/folder_icon.png");
	icon_png = App->textures->LoadTexture("Images/UI/icon_png.png");
	icon_jpg = App->textures->LoadTexture("Images/UI/icon_jpg.png");
	icon_fbx = App->textures->LoadTexture("Images/UI/icon_fbx.png");
	icon_obj = App->textures->LoadTexture("Images/UI/icon_obj.png");
	icon_unknown = App->textures->LoadTexture("Images/UI/icon_unknown.png");

	//directory_see = App->fs->GetMainDirectory();
	//directory_see = "Assets\\";
	directory_see = App->GetCharfromConstChar(App->fs->GetMainDirectory().c_str());
	//directory_see = "";
	//folders = App->fs->Get_AllFolders(directory_see);
	//Iterate_files(folders);
	//ReorderFiles(folders);
	//folders[0].active = true;
	sizeFiles = 50;
	time.Start();
	folders = App->fs->GetAllFoldersNew("", directory_see);
	App->fs->GetAllFiles(directory_see, files);
	return true;
}

update_status Project::Update(float dt)
{
	if (App->fs->CheckAssetsIsModify())
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		GetFolderSee();
	}
	if (active[0].active)
	{
		//ShowProject();
		ShowProject();
	}


	return UPDATE_CONTINUE;
}

bool Project::CleanUp()
{

	return true;
}

std::string Project::GetFolderSee()
{
	std::string temp = fileViwer->begin()._Ptr->directory_name;
	size_t EndName = temp.find_last_of("\\");
	temp = temp.substr(0, EndName + 1);
	return temp;
}

std::vector<Files>* Project::GetFilesSee()
{
	return fileViwer;
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

void Project::ShowProject()
{
	//DeleteFolders(folders);
	//DeleteFiles(files);
	//folders.clear();
	//files.clear();
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
	if (time.ReadSec() > 7)
	{
		time.Start();

		//folders = App->fs->GetAllFoldersNew("", directory_see);
	}

	Folders_update(folders);

	ImGui::NextColumn();
	ImGui::Separator();
	//GetAllFiles
	//files = App->fs->GetAllFilesNew(directory_see);
	App->fs->GetAllFiles(directory_see, files);
	Files_Update(files);

	EndDock();
}

void Project::Folders_update(std::vector<FoldersNew> folders)
{
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.335f, 0.337f, 0.357f, 1.00f));
	for (int i = 0; i < folders.size(); i++)
	{
		ImGui::PushID(i);
		ImGuiTreeNodeFlags node_flags;
		if (folders[i].active)
		{
			node_flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow;
		}
		else
		{
			node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
		}
		if (ImGui::TreeNodeEx(folders[i].file_name, node_flags))
		{
			if (ImGui::BeginPopupContextItem("rename context menu"))
			{
				ImGui::Text("Edit name:");
				ImGui::InputText("##edit2", folders[i].file_name, 256);
				ImGui::EndPopup();
			}
			if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
			{
				//LOG("%.2f - %.2f  / /  %.2f - %.2f", ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y, ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y);
				if (ImGui::IsMouseDoubleClicked(0) && folders[i].active == false)
				{
					SetAllFolderBool(folders, false);
					folders[i].active = true;
					directory_see = App->GetCharfromConstChar(folders[i].directory_name.c_str());
				}
			}
			if (folders[i].folder_child.size() > 0)
			{
				Folders_update(folders[i].folder_child);
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	ImGui::PopStyleColor();
}

void Project::Files_Update(const std::vector<FilesNew>& files)
{
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
					//ChangefileViwer(files[i].parentFolder->folder_child, files[i].file_name);
					directory_see = App->GetCharfromConstChar(files[i].directory_name);
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

void Project::DeleteFiles(std::vector<FilesNew> files)
{
	for (int i = 0; i < files.size(); i++)
	{
		files[i].directory_name = nullptr;
		files[i].directory_name_next = nullptr;
		//delete files[i].file_name;
		files[i].file_name = nullptr;
	}
	files.clear();
}

void Project::DeleteFolders(std::vector<FoldersNew> folders)
{
	for (int i = 0; i < folders.size(); i++)
	{
		folders[i].directory_name.clear();
		//RELEASE(folders[i].file_name);
		folders[i].file_name = nullptr;
		DeleteFolders(folders[i].folder_child);
		folders.clear();
	}
}

void Project::SetAllFolderBool(std::vector<FoldersNew>& folders, bool setBoolean)
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