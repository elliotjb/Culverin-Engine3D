#include "Application.h"
#include "WindowInspector.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Component.h"
#include "CompMesh.h"
#include "CompTransform.h"
#include "CompMaterial.h"
#include "CompCamera.h"

GameObject::GameObject()
{
	Enable();
	uid = App->random->Int();
}

GameObject::GameObject(char* nameGameObject, uint uuid)
{
	Enable();
	uid = uuid;
	name = nameGameObject;
}

GameObject::~GameObject()
{
	//for (int i = 0; i < childs.size(); i++)
	//{
	//	childs[i]->~GameObject();
	//	delete childs[i];
	//}
}

void GameObject::preUpdate(float dt)
{
	if (active)
	{
		//preUpdate Components --------------------------
		for (uint i = 0; i < components.size(); i++)
		{
			if (components[i]->isActive())
			{
				components[i]->preUpdate(dt);
			}
		}

		//preUpdate child Game Objects -------------------
		for (uint i = 0; i < childs.size(); i++)
		{
			if (childs[i]->isActive())
			{
				childs[i]->preUpdate(dt);
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
				//bounding_box->Enclose(mesh->vertices, mesh->num_vertices);
				bounding_box->SetFromCenterAndSize(transform->GetPos(), transform->GetScale()*2);
			}
		}
	}
}

void GameObject::Draw()
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

	if (ImGui::TreeNodeEx(name))
	{
		ImGui::PopStyleColor();
		for (uint i = 0; i < childs.size(); i++)
		{
			childs[i]->ShowHierarchy();
		}

		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}

	if (ImGui::IsItemClicked())
	{
		//Set inspector window of this Game Object
		((Inspector*)App->gui->winManager[INSPECTOR])->LinkObject(this);
		App->camera->SetFocus(this);
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
		ImGui::InputText("##nameModel", name, 256, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine(); App->ShowHelpMarker("Hold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n");
		ImGui::PopStyleVar();

		/* STATIC CHECKBOX */
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 8));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 3));
		
		ImGui::Text(""); ImGui::SameLine(8);
		ImGui::Checkbox("##2", &static_obj);

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

bool GameObject::isVisible() const
{
	bool ret = false;

	CompMesh* mesh = (CompMesh*)FindComponentByType(C_MESH);

	if (mesh != nullptr)
	{
		// If the mesh is Drawing
		if (mesh->isRendering())
		{
			ret = true;
		}

		// If not
		else
		{
			ret = false;
		}
	}

	// If the GameObject hasn't got a mesh, it's allways visible (unless you disable it)
	else
	{
		ret = true;
	}

	return ret;
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
			CompMesh* mesh = new CompMesh(type, this);
			components.push_back(mesh);
			return mesh;
		}

		else if (type == C_TRANSFORM)
		{
			LOG("Adding TRANSFORM COMPONENT.");
			CompTransform* transform = new CompTransform(type, this);
			components.push_back(transform);
			return transform;
		}

		else if (type == C_MATERIAL)
		{
			LOG("Adding MATERIAL COMPONENT.");
			CompMaterial* material = new CompMaterial(type, this);
			components.push_back(material);

			/* Link Material to the Mesh if exists */
			CompMesh* mesh_to_link = (CompMesh*)FindComponentByType(C_MESH);
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

		else if (type == C_CAMERA)
		{
			LOG("Adding CAMERA COMPONENT.");
			CompCamera* camera = new CompCamera(type, this);
			components.push_back(camera);
			return camera;
		}
	}

	return nullptr;
}

void GameObject::AddComponent_(Comp_Type type)
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
			components.push_back(new CompMesh(type, this));
		}

		else if (type == C_TRANSFORM)
		{
			LOG("Adding TRANSFORM COMPONENT.");
			components.push_back(new CompTransform(type, this));
		}

		else if (type == C_MATERIAL)
		{
			LOG("Adding MATERIAL COMPONENT.");
			CompMaterial* material = new CompMaterial(type, this);
			components.push_back(material);

			/* Link Material to the Mesh if exists */
			CompMesh* mesh_to_link = (CompMesh*)FindComponentByType(C_MESH);
			if (mesh_to_link != nullptr)
			{
				mesh_to_link->LinkMaterial(material);
			}
			else
			{
				LOG("MATERIAL not linked to any mesh");
			}
		}

		else if (type == C_CAMERA)
		{
			LOG("Adding CAMERA COMPONENT.");
			components.push_back(new CompCamera(type, this));
		}
	}
}

void GameObject::SaveComponents(JSON_Object* object, std::string name) const
{
	for (int i = 0; i < components.size(); i++)
	{
		std::string temp = name + "Component " + std::to_string(i) + ".";
		components[i]->Save(object, temp);
	}
}

void GameObject::LoadComponents(const JSON_Object* object, std::string name, uint numComponents)
{
	// Frist Add All components by type
	for (int i = 0; i < numComponents; i++)
	{
		std::string temp = name + "Component " + std::to_string(i) + ".";
		Comp_Type type = (Comp_Type)(int)json_object_dotget_number_with_std(object, temp + "Type");
		switch (type)
		{
		case C_UNKNOWN:
			break;
		case C_TRANSFORM:
			this->AddComponent_(C_TRANSFORM);
			break;
		case C_MESH:
			this->AddComponent_(C_MESH);
			break;
		case C_MATERIAL:
			this->AddComponent_(C_MATERIAL);
			break;
		case C_CAMERA:
			this->AddComponent_(C_CAMERA);
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
	if (components[0]->GetType() == C_TRANSFORM)
	{
		return (CompTransform*)components[0];
	}
	else
		return nullptr;
}

uint GameObject::GetNumChilds() const
{
	return childs.size();
}

GameObject* GameObject::GetChildbyIndex(uint pos_inVec) const
{
	return childs[pos_inVec];
}

std::vector<GameObject*> GameObject::GetChildsVec() const
{
	return childs;
}

void GameObject::AddChildGameObject_Copy(GameObject* child)
{
	GameObject* temp = new GameObject(*child);
	temp->uid = App->random->Int();
	temp->parent = this;
	this->childs.push_back(temp);
}

void GameObject::AddChildGameObject_Load(GameObject* child)
{
	child->parent = this;
	this->childs.push_back(child);
}

void GameObject::AddChildGameObject_Replace(GameObject* child)
{
	//GameObject* temp = new GameObject(*child);
	//temp->uid = App->random->Int();
	//temp->parent = this;
	//this->childs.push_back(temp);
}

GameObject* GameObject::GetParent() const
{
	if(parent != nullptr)
		return parent;
	return nullptr;
}

void GameObject::DrawBoundingBox()
{
	//CompTransform* transform = (CompTransform*)FindComponentByType(C_TRANSFORM);
	//if (transform != nullptr)
	//{
	//	glPushMatrix();
	//	glMultMatrixf(transform->GetMultMatrixForOpenGL());
	//}

	glBegin(GL_LINES);
	glLineWidth(3.0f);
	glColor4f(0.25f, 1.0f, 0.0f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(bounding_box->Edge(i).a.x, bounding_box->Edge(i).a.y, bounding_box->Edge(i).a.z);
		glVertex3f(bounding_box->Edge(i).b.x, bounding_box->Edge(i).b.y, bounding_box->Edge(i).b.z);
	}
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//if (transform != nullptr)
	//{
	//	glPopMatrix();
	//}

}

uint GameObject::GetUUID() const
{
	return uid;
}
