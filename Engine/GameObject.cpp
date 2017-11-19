#include "GameObject.h"
#include "Application.h"
#include "WindowInspector.h"
#include "WindowHierarchy.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleFS.h"
#include "Scene.h"
#include "ModuleGUI.h"
#include "ModuleTextures.h"
#include "Component.h"
#include "CompMesh.h"
#include "CompTransform.h"
#include "CompMaterial.h"
#include "CompCamera.h"

GameObject::GameObject(GameObject* parent) :parent(parent)
{
	Enable();
	SetVisible(true);
	uid = App->random->Int();

	if (parent != nullptr)
	{
		parent->childs.push_back(this);
	}
}

GameObject::GameObject(char* nameGameObject, uint uuid)
{
	Enable();
	SetVisible(true);
	uid = uuid;
	name = nameGameObject;
}

GameObject::GameObject(const GameObject& copy)
{
	uid = App->random->Int();
	std::string nametemp = copy.GetName();
	nametemp += " (copy)";
	name = App->GetCharfromConstChar(nametemp.c_str());
	//TODO ->add "(X)" to the name
	active = copy.isActive();
	visible = copy.isVisible();
	static_obj = copy.isStatic();
	bb_active = copy.isAABBActive();
	if (copy.bounding_box != nullptr)
	{
		bounding_box = new AABB(*copy.bounding_box);
	}

	//Create all components from copy object with same data
	for (uint i = 0; i < copy.GetNumComponents(); i++)
	{
		AddComponentCopy(*copy.components[i]);
	}

	//Create childrens from copy object with same data to this new game object
	for (uint i = 0; i < copy.GetNumChilds(); i++)
	{
		//Create from copy constructor all childs of the game object to copy
		childs.push_back(new GameObject(*copy.GetChildbyIndex(i))); 
		childs[i]->parent = this;
	}
}

GameObject::~GameObject()
{
	RELEASE_ARRAY(name);
	delete bounding_box;
	bounding_box = nullptr;
	parent = nullptr;
	components.clear();
	childs.clear();
}

void GameObject::preUpdate(float dt)
{
	if (active)
	{
		//preUpdate Components --------------------------
		for (uint i = 0; i < components.size(); i++)
		{
			if (components[i]->WantDelete())
			{
				DeleteComponent(components[i]);
			}
			else
			{
				if (components[i]->isActive())
				{
					components[i]->preUpdate(dt);
				}
			}
		}

		//preUpdate child Game Objects -------------------
		for (uint i = 0; i < childs.size(); i++)
		{
			if (childs[i]->toDelete)
			{
				App->scene->DeleteGameObject(childs[i]);
			}
			else
			{
				if (childs[i]->isActive())
				{
					childs[i]->preUpdate(dt);
				}
			}
		}
	}
}

void GameObject::Update(float dt)
{
	if (active)
	{
		//Update Components --------------------------
		for (uint i = 0; i < components.size(); i++)
		{
			if (components[i]->isActive())
			{
				components[i]->Update(dt);
			}
		}

		//Update child Game Objects -------------------
		for (uint i = 0; i < childs.size(); i++)
		{
			if (childs[i]->isActive())
			{
				childs[i]->Update(dt);
			}
		}

		// BOUNDING BOX -----------------
		if (bounding_box != nullptr)
		{
			//CompMesh* mesh = (CompMesh*)(FindComponentByType(C_MESH));
			CompTransform* transform = (CompTransform*)(FindComponentByType(C_TRANSFORM));
			if (transform != nullptr)
			{
				//Resize the Bounding Box
				//bounding_box->SetFromCenterAndSize(transform->GetPos(), transform->GetScale()*2);
				box_fixed = *bounding_box;
				box_fixed.TransformAsAABB(transform->GetGlobalTransform());
			}
		}
	}
}

void GameObject::Draw()
{
	if (visible)
	{
		//Draw Components --------------------------
		for (uint i = 0; i < components.size(); i++)
		{
			if (components[i]->isActive())
			{
				components[i]->Draw();
			}
		}

		//Draw child Game Objects -------------------
		for (uint i = 0; i < childs.size(); i++)
		{
			if (childs[i]->isActive())
			{
				childs[i]->Draw();
			}
		}

		if (bb_active)
		{
			// Draw Bounding Box
			DrawBoundingBox();
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

const char* GameObject::GetName() const
{
	return name;
}

void GameObject::ShowHierarchy()
{
	if (!isVisible())
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.385f, 0.385f, 0.385f, 1.00f));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
	}
	bool treeNod = false;
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (((Inspector*)App->gui->winManager[INSPECTOR])->GetSelected() == this)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}
	if (childs.size() == 0)
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	}
	//ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNodeEx(name, node_flags))
	{
		treeNod = true;
	}
	if (ImGui::IsItemClicked())
	{
		//Set inspector window of this Game Object
		((Inspector*)App->gui->winManager[INSPECTOR])->LinkObject(this);
		App->camera->SetFocus(this);
		App->scene->drag = this;
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 6));
	if (ImGui::BeginPopupContextItem("Create"))
	{
		ShowGameObjectOptions();
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
	ImGui::SameLine(); App->ShowHelpMarker("Right Click to open Options");
	if(treeNod)
	{
		ImGui::PopStyleColor();

		if (App->scene->drag != nullptr)
		{
			if (ImGui::IsItemHoveredRect())
			{
				if (App->scene->drag != this)
				{
					ImGui::BeginTooltip();
					ImGui::Text("Move %s to %s", App->scene->drag->GetName(), this->GetName());
					ImGui::EndTooltip();
				}
			}
		}

		for (uint i = 0; i < childs.size(); i++)
		{
			ImGui::PushID(i);
			childs[i]->ShowHierarchy();
			ImGui::PopID();
		}

		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}
}

void GameObject::ShowGameObjectOptions()
{
	//Create child Game Objects / Components
	if (ImGui::MenuItem("Copy"))
	{
		((Hierarchy*)App->gui->winManager[WindowName::HIERARCHY])->SetGameObjectCopy(this);
	}
	if (ImGui::MenuItem("Paste", NULL, false, false))
	{
		// TODO ELLIOT
		//parent->AddChildGameObject_Copy(((Hierarchy*)App->gui->winManager[WindowName::HIERARCHY])->GetCopied());
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Rename", NULL, false, false))
	{
		// Not functional
	}
	if (ImGui::MenuItem("Duplicate", NULL, false, false))
	{
		// GetParent()->AddChildGameObject_Copy(this);
	}
	if (ImGui::MenuItem("Delete"))
	{
		((Hierarchy*)App->gui->winManager[WindowName::HIERARCHY])->SetGameObjecttoDelete(this);
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Create Empty"))
	{
		GameObject* empty = App->scene->CreateGameObject(this);
		((Inspector*)App->gui->winManager[WindowName::INSPECTOR])->LinkObject(empty);
	}
	if (ImGui::MenuItem("Cube"))
	{
		GameObject* cube = App->scene->CreateCube(this);
		((Inspector*)App->gui->winManager[WindowName::INSPECTOR])->LinkObject(cube);
	}
	if (ImGui::MenuItem("Sphere"))
	{
		GameObject* sphere = App->scene->CreateSphere(this);
		((Inspector*)App->gui->winManager[WindowName::INSPECTOR])->LinkObject(sphere);
	}
	ImGui::Separator();
	ImGui::MenuItem("ADD COMPONENT", NULL, false, false);
	if (ImGui::MenuItem("Transform"))
	{
		AddComponent(Comp_Type::C_TRANSFORM);
	}
	if (ImGui::MenuItem("Mesh"))
	{
		AddComponent(Comp_Type::C_MESH);
	}
	if (ImGui::MenuItem("Material"))
	{
		AddComponent(Comp_Type::C_MATERIAL);
	}
}

void GameObject::ShowInspectorInfo()
{
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.211f, 0.211f, 0.211f, 1.00f));
	if (ImGui::BeginChild(ImGui::GetID("Inspector"), ImVec2(ImGui::GetWindowWidth(), 90)))
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
		char namedit[50];
		strcpy_s(namedit, 50, name);
		if (ImGui::InputText("##nameModel", namedit, 50, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			name = App->fs->ConverttoChar(std::string(namedit).c_str());
		}
		//ImGui::InputText("##nameModel", name, 256, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine(); App->ShowHelpMarker("Hold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n");
		ImGui::PopStyleVar();

		/* STATIC CHECKBOX */
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 8));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 3));
		
		static bool window_active = false;
		static bool static_object = false;
		ImGui::Text(""); ImGui::SameLine(8);
		if (ImGui::Checkbox("##2", &static_obj))
		{
			if (App->engineState == EngineState::STOP)
			{
				window_active = true;
				static_object = static_obj;
				static_obj = !static_obj;
			}
			else
			{
				window_active = false;
				static_obj = !static_obj;
			}
		}

		if (window_active)
		{
			if (childs.size() > 0)
			{
				// Show window to change childs to static
				ShowFreezeChildsWindow(static_object, window_active);
			}
			else
			{
				FreezeTransforms(static_object, false);
				window_active = false;
			}
		}
		ImGui::SameLine();
		ImGui::PopStyleVar();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "Static");
		ImGui::PopStyleVar();
	}

	/* BOUNDING BOX CHECKBOX */
	if (bounding_box != nullptr)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 8));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 3));
		
		ImGui::Text(""); ImGui::SameLine(8);
		ImGui::Checkbox("##3", &bb_active);

		ImGui::SameLine();
		ImGui::PopStyleVar();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "Bounding Box");
		ImGui::PopStyleVar();
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();

	// UPDATE EDITOR WINDOWS OF EACH COMPONENT
	for (uint i = 0; i < components.size(); i++)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
		bool open = false;
		if (ImGui::TreeNodeEx(components[i]->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			open = true;
		}
		ImGui::PopStyleColor();
		if (ImGui::BeginPopupContextItem("Open"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 3));
			components[i]->ShowOptions();
			ImGui::PopStyleVar();
			ImGui::EndPopup();
		}
		if(open)
		{
			components[i]->ShowInspectorInfo();
			ImGui::Separator();
		}
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("");
	ImGui::SameLine(ImGui::GetWindowWidth() / 2 - Width_AddComponent / 2);
	//ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2);
	static bool add_component = false;
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.208f, 0.208f, 0.208f, 1.00f));
	if (ImGui::ButtonEx("ADD COMPONENT", ImVec2(Width_AddComponent, 20)))
	{
		add_component = !add_component;
	}
	ImGui::PopStyleColor();
	ImVec2 pos_min = ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y);
	ImVec2 pos_max = ImVec2(pos_min.x + (ImGui::GetWindowWidth() / 2), pos_min.y + (Width_AddComponent / 2));
	if (add_component)
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.258f, 0.258f, 0.258f, 1.00f));
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y + ImGui::GetItemRectSize().y));
		ImGui::SetNextWindowSize(ImVec2(Width_AddComponent, Width_AddComponent / 2));
		ImGui::Begin("AddComponent", NULL, ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.311f, 0.311f, 0.311f, 1.00f));
		if (ImGui::BeginChild(ImGui::GetID("AddComponent"), ImVec2(ImGui::GetWindowWidth(), 20), false, ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImGui::Text("");
			ImGui::SameLine(62);
			ImGui::Text("Component");
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
		if (ImGui::MenuItem("Transform"))
		{
			AddComponent(Comp_Type::C_TRANSFORM);
			add_component = false;
		}
		if (ImGui::MenuItem("Mesh"))
		{
			AddComponent(Comp_Type::C_MESH);
			add_component = false;
		}
		if (ImGui::MenuItem("Material"))
		{
			AddComponent(Comp_Type::C_MATERIAL);
			add_component = false;
		}
		if (ImGui::MenuItem("Camera"))
		{
			AddComponent(Comp_Type::C_CAMERA);
			add_component = false;
		}
		ImGui::End();
		ImGui::PopStyleColor();
	}
	if (ImGui::IsMouseHoveringRect(pos_min, pos_max) == false)
	{
		if(ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1) || ImGui::IsMouseClicked(2))
			add_component = false;
	}
}

void GameObject::FreezeTransforms(bool freeze, bool change_childs)
{
	if (GetComponentTransform() != nullptr)
	{
		static_obj = freeze;
		GetComponentTransform()->Freeze(freeze);
		
		if (change_childs)
		{
			for (uint i = 0; i < childs.size(); i++)
			{
				childs[i]->static_obj = freeze;
				childs[i]->FreezeTransforms(freeze, change_childs);
			}
		}
	}
}

void GameObject::ShowFreezeChildsWindow(bool freeze, bool& active)
{
	ImGui::OpenPopup("Change Static Flags");
	if (ImGui::BeginPopupModal("Change Static Flags", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (freeze)
		{
			ImGui::TextWrapped("Do you want to enable the static flags for all the child objects as well?");
		}
		else
		{
			ImGui::TextWrapped("Do you want to disable the static flags for all the child objects as well?");
		}
		ImGui::Spacing();
		if (ImGui::Button("Yes change children", ImVec2(140, 0)))
		{
			FreezeTransforms(freeze, true);
			active = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("No, this object only", ImVec2(130, 0)))
		{
			FreezeTransforms(freeze, false);
			active = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(80, 0)))
		{
			static_obj = !freeze;
			active = false;
		}
	}
	ImGui::EndPopup();
}

void GameObject::SetVisible(bool visible)
{
	this->visible = visible;
}

void GameObject::SetStatic(bool set_static)
{
	static_obj = set_static;
}

bool GameObject::isActive() const
{
	return active;
}

bool GameObject::isVisible() const
{
	return visible;
}

bool GameObject::isStatic() const
{
	return static_obj;
}

Component* GameObject::FindComponentByType(Comp_Type type) const
{
	Component* comp = nullptr;

	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type) // We need to check if the component is ACTIVE first?�
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
		if (components[i]->GetType() == type) //We need to check if the component is ACTIVE first?�
		{
			dupe = true;
			LOG("There's already one component of this type in '%s'.", name);
			break;
		}
	}

	if (!dupe)
	{
		if (type == Comp_Type::C_MESH)
		{
			LOG("Adding MESH COMPONENT.");
			CompMesh* mesh = new CompMesh(type, this);
			components.push_back(mesh);
			/* Link Material to the Mesh if exists */
			CompMaterial* material_link = (CompMaterial*)FindComponentByType(Comp_Type::C_MATERIAL);
			if (material_link != nullptr)
			{
				mesh->LinkMaterial(material_link);
			}
			else
			{
				LOG("Havent Material");
			}
			return mesh;
		}

		else if (type == Comp_Type::C_TRANSFORM)
		{
			LOG("Adding TRANSFORM COMPONENT.");
			CompTransform* transform = new CompTransform(type, this);
			components.push_back(transform);
			return transform;
		}

		else if (type == Comp_Type::C_MATERIAL)
		{
			LOG("Adding MATERIAL COMPONENT.");
			CompMaterial* material = new CompMaterial(type, this);
			components.push_back(material);

			/* Link Material to the Mesh if exists */
			CompMesh* mesh_to_link = (CompMesh*)FindComponentByType(Comp_Type::C_MESH);
			if (mesh_to_link != nullptr)
			{
				mesh_to_link->LinkMaterial(material);
			}
			else
			{
				LOG("MATERIAL not linked to any mesh");
			}
			return material;
		}

		else if (type == Comp_Type::C_CAMERA)
		{
			LOG("Adding CAMERA COMPONENT.");
			CompCamera* camera = new CompCamera(type, this);
			components.push_back(camera);
			return camera;
		}
	}

	return nullptr;
}

void GameObject::AddComponentCopy(const Component& copy)
{
	switch (copy.GetType())
	{
	case (Comp_Type::C_TRANSFORM):
	{
		CompTransform* transform = new CompTransform((CompTransform&)copy, this); //Transform copy constructor
		components.push_back(transform);
		break;
	}
	case (Comp_Type::C_MESH):
	{
		CompMesh* mesh = new CompMesh((CompMesh&)copy, this); //Mesh copy constructor
		components.push_back(mesh);
		/* Link Material to the Mesh if exists */
		CompMaterial* material_link = (CompMaterial*)FindComponentByType(Comp_Type::C_MATERIAL);
		if (material_link != nullptr)
		{
			mesh->LinkMaterial(material_link);
		}
		else
		{
			LOG("Havent Material");
		}
		break;
	}
	case (Comp_Type::C_MATERIAL):
	{
		CompMaterial* material = new CompMaterial((CompMaterial&)copy, this); //Material copy constructor
		components.push_back(material);
		/* Link Material to the Mesh if exists */
		CompMesh* mesh_to_link = (CompMesh*)FindComponentByType(Comp_Type::C_MESH);
		if (mesh_to_link != nullptr)
		{
			mesh_to_link->LinkMaterial(material);
		}
		else
		{
			LOG("MATERIAL not linked to any mesh");
		}
		break;
	}
	case (Comp_Type::C_CAMERA):
	{
		CompCamera* transform = new CompCamera((CompCamera&)copy, this); //Camera copy constructor
		components.push_back(transform);
		break;
	}
	default:
		break;
	}
}

void GameObject::SaveComponents(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const
{
	for (int i = 0; i < components.size(); i++)
	{
		std::string temp = name + "Component " + std::to_string(i) + ".";
		components[i]->Save(object, temp, saveScene, countResources);
	}
}

void GameObject::LoadComponents(const JSON_Object* object, std::string name, uint numComponents)
{
	// First Add All components by type
	for (int i = 0; i < numComponents; i++)
	{
		std::string temp = name + "Component " + std::to_string(i) + ".";
		Comp_Type type = (Comp_Type)(int)json_object_dotget_number_with_std(object, temp + "Type");
		switch (type)
		{
		case C_UNKNOWN:
			break;
		case C_TRANSFORM:
			this->AddComponent(C_TRANSFORM);
			break;
		case C_MESH:
			this->AddComponent(C_MESH);
			break;
		case C_MATERIAL:
			this->AddComponent(C_MATERIAL);
			break;
		case C_CAMERA:
			this->AddComponent(C_CAMERA);
			break;
		default:
			break;
		}
	}

	// Now Iterate All components and Load variables
	for (int i = 0; i < components.size(); i++)
	{
		std::string temp = name + "Component " + std::to_string(i) + ".";
		components[i]->Load(object, temp);
	}
}

int GameObject::GetNumComponents() const
{
	return components.size();
}

CompTransform* GameObject::GetComponentTransform() const
{
	return (CompTransform*)FindComponentByType(Comp_Type::C_TRANSFORM);
}

CompMesh* GameObject::GetComponentMesh() const
{
	return (CompMesh*)FindComponentByType(Comp_Type::C_MESH);
}

CompMaterial* GameObject::GetComponentMaterial() const
{
	return (CompMaterial*)FindComponentByType(Comp_Type::C_MATERIAL);
}

Component* GameObject::GetComponentbyIndex(uint i) const
{
	if (i < components.size() && i >= 0)
	{
		return components[i];
	}
	return nullptr;
}

void GameObject::DeleteAllComponents()
{
	for (int i = 0; i < components.size(); i++)
	{
		delete components[i];
	}
	components.clear();
}

void GameObject::DeleteComponent(Component* component)
{
	if (component != nullptr && components.size() > 0)
	{
		std::vector<Component*>::iterator item = components.begin();
		for (int i = 0; i < components.size(); i++)
		{
			if (component == components[i])
			{
				components.erase(item);
				break;
			}
			item++;
		}
		RELEASE(component);
	}
}

uint GameObject::GetNumChilds() const
{
	return childs.size();
}

GameObject* GameObject::GetChildbyIndex(uint pos_inVec) const
{
	return childs[pos_inVec];
}

GameObject* GameObject::GetChildbyName(const char* name) const
{
	if (childs.size() > 0)
	{
		for (int i = 0; i < childs.size(); i++)
		{
			if (strcmp(childs[i]->GetName(), name) == 0)
			{
				return childs[i];
			}
		}
	}
	return nullptr;
}

uint GameObject::GetIndexChildbyName(const char * name) const
{
	if (childs.size() > 0)
	{
		for (int i = 0; i < childs.size(); i++)
		{
			if (strcmp(childs[i]->GetName(), name) == 0)
			{
				return i;
			}
		}
	}
	return 0;
}

void GameObject::RemoveChildbyIndex(uint index)
{
	if (childs.size() > 0)
	{
		std::vector<GameObject*>::iterator item = childs.begin();
		for (int i = 0; i < childs.size(); i++)
		{
			if (i == index)
			{
				childs.erase(item);
				break;
			}
			item++;
		}
	}
}

std::vector<GameObject*> GameObject::GetChildsVec() const
{
	return childs;
}

std::vector<GameObject*>* GameObject::GetChildsPtr() 
{
	return &childs;
}

void GameObject::AddChildGameObject_Copy(const GameObject* child)
{
	GameObject* temp = new GameObject(*child);
	temp->uid = App->random->Int();
	std::string temp_name = temp->name;
	temp_name += " (1)";
	temp->name = App->GetCharfromConstChar(temp_name.c_str());
	temp_name.clear();
	temp->parent = this;
	childs.push_back(temp);
}

void GameObject::AddChildGameObject_Load(GameObject* child)
{
	child->parent = this;
	childs.push_back(child);
}

void GameObject::AddChildGameObject_Replace(GameObject* child)
{
	child->parent = this;
	childs.push_back(child);
	App->scene->gameobjects.pop_back();
}

void GameObject::UpdateChildsMatrices()
{
	for (uint i = 0; i < childs.size(); i++)
	{
		childs[i]->GetComponentTransform()->UpdateGlobalMatrixRecursive();
	}
}

GameObject* GameObject::GetParent() const
{
	return parent;
}

bool GameObject::HaveParent()
{
	return (parent != nullptr);
}

void GameObject::AddBoundingBox(const ResourceMesh* mesh)
{
	if (bounding_box == nullptr)
	{
		bounding_box = new AABB();
	}
	bounding_box->SetNegativeInfinity();
	bounding_box->Enclose(mesh->vertices, mesh->num_vertices);
}

void GameObject::DrawBoundingBox()
{
	glBegin(GL_LINES);
	glLineWidth(3.0f);
	glColor4f(0.25f, 1.0f, 0.0f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(box_fixed.Edge(i).a.x, box_fixed.Edge(i).a.y, box_fixed.Edge(i).a.z);
		glVertex3f(box_fixed.Edge(i).b.x, box_fixed.Edge(i).b.y, box_fixed.Edge(i).b.z);
	}
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void GameObject::SetAABBActive(bool active)
{
	bb_active = active;
}

bool GameObject::isAABBActive() const
{
	return bb_active;
}

uint GameObject::GetUUID() const
{
	return uid;
}

void GameObject::SetUUID(uint uuid)
{
	uid = uuid;
}

void GameObject::SetUUIDRandom()
{
	uid = App->random->Int();
}

bool GameObject::WanttoDelete() const
{
	return toDelete;
}

void GameObject::SettoDelete()
{
	toDelete = true;
}
