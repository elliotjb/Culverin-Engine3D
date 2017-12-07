#include "CompScript.h"
#include "Application.h"
#include "Scene.h"

CompScript::CompScript(Comp_Type t, GameObject* parent) : Component(t, parent)
{
	uid = App->random->Int();
}

CompScript::CompScript(const CompScript & copy, GameObject * parent) : Component(Comp_Type::C_SCRIPT, parent)
{
	nameComponent = copy.nameScript.c_str();
}

CompScript::~CompScript()
{
	RELEASE(editor);
}

void CompScript::Init()
{
	nameComponent = "New Sript (Script)";
	nameScript = "New Sript";
	editor = new Script_editor();
	editor->Start(nameScript.c_str());
	editor->SaveScript();
}

void CompScript::preUpdate(float dt)
{
	//Check if have public in script
	//std::string allscript = editor->editor.GetText();
	//size_t firstPublic = allscript.find_first_of("public");
}

void CompScript::Update(float dt)
{
}

void CompScript::ShowOptions()
{
	if (ImGui::MenuItem("Reset", NULL, false, false))
	{
		// Not implmented yet.
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Remove Component"))
	{
		toDelete = true;
	}
	if (ImGui::MenuItem("Move to Front", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Move to Back", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Move Up", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Move Down", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Copy Component", NULL, false, false))
	{
		// Component* Copy = this;
	}
	if (ImGui::MenuItem("Paste Component", NULL, false, false))
	{
		//parent->AddComponent(App->scene->copyComponent->GetType())
		// Create contructor Component Copy or add funtion to add info
	}
}

void CompScript::ShowInspectorInfo()
{
	// Reset Values Button -------------------------------------------
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 0));
	ImGui::SameLine(ImGui::GetWindowWidth() - 26);
	if (ImGui::ImageButton((ImTextureID*)App->scene->icon_options_transform, ImVec2(13, 13), ImVec2(-1, 1), ImVec2(0, 0)))
	{
		ImGui::OpenPopup("OptionsScript");
	}

	// Button Options --------------------------------------
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.00f));
	if (ImGui::BeginPopup("OptionsScript"))
	{
		/* Reset Material */
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
		if (ImGui::Button("Reset Script"))
		{

			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}

	ImGui::Text("Script"); ImGui::SameLine();
	ImGui::Selectable(nameScript.c_str(), false);
	static bool activeScript = false;
	if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
	{
		//LOG("%.2f - %.2f  / /  %.2f - %.2f", ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y, ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y);
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringWindow())
		{
			activeScript = !activeScript;
		}
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	if (activeScript)
	{
		editor->Show(activeScript);
	}

	ImGui::TreePop();
}

void CompScript::Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const
{
	json_object_dotset_number_with_std(object, name + "Type", Comp_Type::C_SCRIPT);
	json_object_dotset_number_with_std(object, name + "UUID", uid);
	json_object_dotset_string_with_std(object, name + "Name Script", nameScript.c_str());
}

void CompScript::Load(const JSON_Object* object, std::string name)
{
	uid = json_object_dotget_number_with_std(object, name + "UUID");
	nameScript = json_object_dotget_string_with_std(object, name + "Name Script");
	std::string temp = nameScript + " (Script)";
	nameComponent = "Script";
	editor = new Script_editor();
	editor->Start(nameScript.c_str(), false);
	Enable();
}