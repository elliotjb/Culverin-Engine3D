#include "Application.h"
#include "ModuleInspector.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Component.h"
#include "CompMesh.h"
#include "CompTransform.h"

GameObject::GameObject()
{
	Enable();
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	if (active)
	{
		//Update Components --------------------------
		for (uint i = 0; i < components.size(); i++)
		{
			if (components[i]->isActive())
			{
				components[i]->Update();
			}
		}

		//Update child Game Objects -------------------
		for (uint i = 0; i < childs.size(); i++)
		{
			if (childs[i]->isActive())
			{
				childs[i]->Update();
			}
		}
	}
}

bool GameObject::Enable()
{
	if (!active)
	{
		active = true;
	}
	return active;
}

bool GameObject::Disable()
{
	if (active)
	{
		active = false;
	}
	return active;
}

void GameObject::SetName(char * name)
{
	this->name = name;
}

void GameObject::ShowHierarchy()
{
	if (ImGui::TreeNodeEx(name))
	{
		for (uint i = 0; i < childs.size(); i++)
		{
			childs[i]->ShowHierarchy();
		}

		ImGui::TreePop();
	}

	if (ImGui::IsItemClicked())
	{
		//Set inspector window of this Game Object
		((Inspector*)App->gui->winManager[INSPECTOR])->LinkObject(this);
	}
}

void GameObject::ShowInspectorInfo()
{
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowWidth(), 40));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.211f, 0.211f, 0.211f, 1.00f));

	if (ImGui::BeginChild(ImGui::GetID("Inspector"), ImVec2(ImGui::GetWindowWidth(), 40)))
	{
		static GLuint icon_GameObject = App->textures->LoadTexture("Images/UI/icon_GameObject.png");
		ImGui::Spacing();

		ImGui::Text(""); ImGui::SameLine(5);
		ImGui::Image((ImTextureID*)icon_GameObject, ImVec2(23, 23), ImVec2(-1, 1), ImVec2(0, 0)); ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 8));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 3));

		/* ENABLE-DISABLE CHECKBOX*/
		ImGui::Checkbox("##1", &active);

		/* NAME OF THE GAMEOBJECT */
		ImGui::SameLine();
		ImGui::PopStyleVar();
		ImGui::InputText("##nameModel", name, 256, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine(); App->ShowHelpMarker("Hold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n");
		ImGui::PopStyleVar();
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::Separator();

	for (uint i = 0; i < components.size(); i++)
	{
		components[i]->ShowInspectorInfo();
		ImGui::Separator();
	}
}

bool GameObject::isActive() const
{
	return active;
}

Component* GameObject::FindComponentByType(Comp_Type type)
{
	Component* comp = nullptr;

	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type) //We need to check if the component is ACTIVE first?¿
		{
			comp = components[i];
			break;
		}
	}

	return comp;
}

Component* GameObject::AddComponent(Comp_Type type)
{
	bool dupe = false;
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type) //We need to check if the component is ACTIVE first?¿
		{
			dupe = true;
			LOG("There's already one component of this type in the Game Object.");
			break;
		}
	}

	if (!dupe)
	{
		if (type == C_MESH)
		{
			LOG("Adding MESH COMPONENT.");
			CompMesh* mesh = new CompMesh(type);
			components.push_back(mesh);
			return mesh;
		}

		if (type == C_TRANSFORM)
		{
			LOG("Adding TRANSFORM COMPONENT.");
			CompTransform* transform = new CompTransform(type);
			components.push_back(transform);
			return transform;
		}
	}

	return nullptr;
}
