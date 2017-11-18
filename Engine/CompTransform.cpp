#include "CompTransform.h"
#include "Component.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "CompCamera.h"
#include "ModuleFS.h"
#include "Scene.h"
#include "ModuleConsole.h"
#include "ModuleWindow.h"
#include "WindowInspector.h"
#include "WindowSceneWorld.h"
#include "ImGui/ImGuizmo.h"
#include <gl/GL.h>
#include <gl/GLU.h>

CompTransform::CompTransform(Comp_Type t, GameObject* parent) :Component(t, parent)
{
	nameComponent = "Transformation";
}

CompTransform::~CompTransform()
{
}

void CompTransform::Init(float3 p, float4 r, float3 s)
{
	SetPos(p);
	SetRot(Quat(r.x, r.y, r.z, r.w));
	SetScale(s);
}

void CompTransform::Init(float3 p, float3 r, float3 s)
{
	SetPos(p);
	SetRot(r);
	SetScale(s);
}

void CompTransform::Update(float dt)
{
	//if (((Inspector*)App->gui->winManager[INSPECTOR])->GetSelected() == parent)
	//{
	//	ImGuizmo::Enable(true);

	//	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

	//	screen = ((SceneWorld*)App->gui->winManager[SCENEWORLD])->GetWindowParams();
	//	ImGuizmo::SetRect(screen.x, screen.y, screen.z, screen.w);

	//	global_transposed = global_transform.Transposed();

	//	// SET GUIZMO OPERATION ----------------------------------
	//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	//	{
	//		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	//	}
	//	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	//	{
	//		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	//	}
	//	else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	//	{
	//		mCurrentGizmoOperation = ImGuizmo::SCALE;
	//	}

	//	ImGuizmo::Manipulate(App->camera->GetViewMatrix(), App->camera->GetProjMatrix(), mCurrentGizmoOperation, transform_mode, global_transposed.ptr(), NULL, NULL);
	//	
	//	if (ImGuizmo::IsUsing())
	//	{
	//		global_transposed.Transpose();
	//		global_transposed.Decompose(position_global, rotation_global, scale);
	//		rotation_euler_global = rotation.ToEulerXYZ() * RADTODEG;
	//			
	//		toUpdate = true;
	//	}
	//}

	if (toUpdate)
	{
		App->console->ClearLog();
		UpdateMatrix(transform_mode);
		toUpdate = false;
		LOG("LOCAL ---------------");
		LOG("x:%.3f y:%.3f z:%.3f POSITION ", position.x, position.y, position.z);
		LOG("x:%.3f y:%.3f z:%.3f w:%.3f ROTATION", rotation_euler.x, rotation_euler.y, rotation_euler.z);
		LOG("x:%.3f y:%.3f z:%.3f SCALE", scale.x, scale.y, scale.z);
		LOG("GLOBAL --------------");
		LOG("x:%.3f y:%.3f z:%.3f POSITION_G ", position_global.x, position_global.y, position_global.z);
		LOG("x:%.3f y:%.3f z:%.3f w:%.3f ROTATION_G ", rotation_euler_global.x, rotation_euler_global.y, rotation_euler_global.z);
		LOG("x:%.3f y:%.3f z:%.3f SCALE", scale.x, scale.y, scale.z);
	}
}

void CompTransform::ShowOptions()
{
	//ImGui::MenuItem("CREATE", NULL, false, false);
	if (ImGui::MenuItem("Reset"))
	{
		// Not implmented yet.
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Move to Front", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Move to Back", NULL, false, false))
	{
		// Not implmented yet.
	}
	if (ImGui::MenuItem("Copy Component"))
	{
		// Component* Copy = this;
	}
	if (ImGui::MenuItem("Paste Component", NULL, false, false))
	{
		//parent->AddComponent(App->scene->copyComponent->GetType())
		// Create contructor Component Copy or add funtion to add info
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Reset Position"))
	{
		SetPos(float3::zero);
		ImGui::CloseCurrentPopup();
	}
	if (ImGui::MenuItem("Reset Rotation", NULL, false, false))
	{
		SetRot(math::Quat::identity);
		ImGui::CloseCurrentPopup();
	}
	if (ImGui::MenuItem("Reset Scale"))
	{
		SetScale(math::float3(1, 1, 1));
		ImGui::CloseCurrentPopup();
	}
}

void CompTransform::ShowInspectorInfo()
{
	// Reset Values Button -------------------------------------------
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 0));
	ImGui::SameLine(ImGui::GetWindowWidth() - 26);
	if (ImGui::ImageButton((ImTextureID*)App->scene->icon_options_transform, ImVec2(13, 13), ImVec2(-1, 1), ImVec2(0, 0)))
	{
		ImGui::OpenPopup("OptionsTransform");
	}
	ImGui::PopStyleVar();

	// Options Button --------------------------------------------------
	//ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.00f));
	if (ImGui::BeginPopup("OptionsTransform"))
	{
		ShowOptions();
		ImGui::EndPopup();
	}

	// SET TRANSFORM MODE -------------------------------------------------------
	if (ImGui::RadioButton("Local", transform_mode == ImGuizmo::LOCAL))
	{
		transform_mode = ImGuizmo::LOCAL;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("World", transform_mode == ImGuizmo::WORLD))
	{
		transform_mode = ImGuizmo::WORLD;
	}

	// SHOW OUTPUT (depending on mode and if transform is freezed) ---------------
	if (App->engineState == EngineState::STOP)
	{
		ShowTransform(0.5f); // 0.5f = drag speed of editor variables of the transform
	}
	else
	{
		if (freeze)
		{
			ShowTransform(0.0f);  // 0.0f = no editable
		}
		else
		{
			ShowTransform(0.5f);
		}
	}
	// ------------------------------------------------------------------
	// This function let mouse trespassing the screen to enter from the opposite side
	//if (1)
	//{
	//	if (App->input->GetMouseXGlobal() <= 1680 &&
	//		App->input->GetMouseXGlobal() > 1680 - 10)
	//	{
	//		SetCursorPos(30, App->input->GetMouseYGlobal());
	//	}
	//	if (App->input->GetMouseXGlobal() >= 0 &&
	//		App->input->GetMouseXGlobal() < 10)
	//	{
	//		SetCursorPos(width - 20, App->input->GetMouseYGlobal());
	//	}
	//}
	// -------------------------------------------------------------------------------
//	ImGui::TreePop();
	ImGui::TreePop();
}



void CompTransform::ShowTransform(float drag_speed)
{
	int op = ImGui::GetWindowWidth() / 4;

	// Depending transform mode, edit local/world matrix.
	switch (transform_mode)
	{
	case (ImGuizmo::MODE::LOCAL):
	{
		ImGui::Text("Position"); ImGui::SameLine(op + 30);
		if (ImGui::DragFloat3("##pos", &position[0], drag_speed))
		{
			SetPos(position);
		}
		ImGui::Text("Rotation"); ImGui::SameLine(op + 30);
		if (ImGui::DragFloat3("##rot", &rotation_euler[0], drag_speed))
		{
			SetRot(rotation_euler);
		}
		break;
	}
	case (ImGuizmo::MODE::WORLD):
	{
		ImGui::Text("Position"); ImGui::SameLine(op + 30);
		if (ImGui::DragFloat3("##pos", &position_global[0], drag_speed))
		{
			SetPosGlobal(position_global);
		}
		ImGui::Text("Rotation"); ImGui::SameLine(op + 30);
		if (ImGui::DragFloat3("##rot", &rotation_euler_global[0], drag_speed))
		{
			SetRotGlobal(rotation_euler_global);
		}
		break;
	}
	default:
		break;
	}

	ImGui::Text("Scale"); ImGui::SameLine(op + 30);
	if (ImGui::DragFloat3("##scal", &scale[0], drag_speed))
	{
		SetScale(scale);
	}
}

void CompTransform::SetPosGlobal(float3 pos)
{
	position_global = pos;
	toUpdate = true;
}

void CompTransform::SetRotGlobal(float3 rot_g)
{
	rotation_global = Quat::FromEulerXYZ(rot_g[0] * DEGTORAD, rot_g[1] * DEGTORAD, rot_g[2] * DEGTORAD);
	toUpdate = true;
}

void CompTransform::SetPos(float3 pos_g)
{
	position = pos_g;
	toUpdate = true;
}

void CompTransform::SetRot(Quat rot)
{
	rotation_euler = rot.ToEulerXYZ() * RADTODEG;
	rotation = rot;
	toUpdate = true;
}

void CompTransform::SetRot(float3 rot)
{
	rotation = Quat::FromEulerXYZ(rot[0] * DEGTORAD, rot[1] * DEGTORAD, rot[2] * DEGTORAD);
	toUpdate = true;
}

void CompTransform::SetScale(float3 scal)
{
	scale = scal;
	toUpdate = true;
}

void CompTransform::ResetMatrix()
{
	SetPos(float3::zero);
	SetRot(float3::zero);
	rotation_euler = float3::zero;
	SetScale(float3(1, 1, 1));
	toUpdate = true;
}

void CompTransform::SetLocalTransform()
{
	local_transform = float4x4::FromTRS(position, rotation, scale);
}

void CompTransform::SetGlobalTransform()
{
	global_transform = float4x4::FromTRS(position_global, rotation_global, scale);
}

void CompTransform::UpdateLocalTransform()
{
	local_transform = float4x4::identity;
	const GameObject* parent_object = parent;

	//If it has no parents, local transform is the same as the global transform
	if (parent_object->GetParent() == nullptr)
	{
		local_transform = global_transform;		
	}
	//If not, apply parent global transform (Inverted) to get it's local transform
	else
	{
		CompTransform* parent_transform = parent_object->GetParent()->GetComponentTransform();
		local_transform = parent_transform->GetGlobalTransform().Inverted() * local_transform;
	}

	// Fill the output variables from the updated matrix
	local_transform.Decompose(position, rotation, scale);
	rotation_euler = rotation.ToEulerXYZ() * RADTODEG;
}

void CompTransform::UpdateGlobalTransform()
{
	global_transform = float4x4::identity;
	std::list<const GameObject*> parents;
	const GameObject* parent_object = parent;

	// Put all parents of the game object to pass from local to global matrix
	while (parent_object != nullptr)
	{
		parents.push_back(parent_object);
		parent_object = parent_object->GetParent();
	}

	std::list<const GameObject*>::reverse_iterator item = parents.rbegin();
	
	// Multiply parents local transforms to get the Global transform of this game object
	while (item !=  parents.crend())
	{
		float4x4 matrix = (*item)->GetComponentTransform()->GetLocalTransform();
		global_transform = global_transform * matrix;
		item++;
	}

	//Fill the output variables from the updated matrix
	global_transform.Decompose(position_global, rotation_global, scale);
	rotation_euler_global = rotation_global.ToEulerXYZ() * RADTODEG;
}

void CompTransform::UpdateGlobalMatrixRecursive()
{
	UpdateGlobalTransform();
	parent->UpdateChildsMatrices();
}

void CompTransform::UpdateMatrix(ImGuizmo::MODE mode)
{
	switch (mode)
	{
	case (ImGuizmo::MODE::LOCAL): 
	{
		SetLocalTransform(); /*First, set local transform from inspector variables*/
		UpdateGlobalTransform(); /*Then, update global matrix*/
		break;
	}
	case (ImGuizmo::MODE::WORLD):
	{
		SetGlobalTransform(); /*First, set global transform from inspector variables*/
		UpdateLocalTransform(); /*Then, update local matrix*/
		break;
	}
	default:
		break;
	}

	// Update Global matrices of all children
	parent->UpdateChildsMatrices();
}

float3 CompTransform::GetPos() const
{
	return position;
}

Quat CompTransform::GetRot() const
{
	return rotation;
}

float3 CompTransform::GetScale() const
{
	return scale;
}

float4x4 CompTransform::GetLocalTransform() const
{
	return local_transform;
}

float4x4 CompTransform::GetGlobalTransform() const
{
	return global_transform;
}

ImGuizmo::MODE CompTransform::GetMode() const
{
	return transform_mode;
}

void CompTransform::Freeze(bool freeze)
{
	this->freeze = freeze;
}

const float* CompTransform::GetMultMatrixForOpenGL() const
{
	return global_transform.Transposed().ptr();
}

void CompTransform::Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const
{
	// TRANSFORM-----------
	json_object_dotset_number_with_std(object, name + "Type", C_TRANSFORM);
	// Position
	App->fs->json_array_dotset_float3(object, name + "Position", GetPos());
	// Rotation
	App->fs->json_array_dotset_float4(object, name + "Rotation", float4(GetRot().x, GetRot().y, GetRot().z, GetRot().w));
	// Scale
	App->fs->json_array_dotset_float3(object, name + "Scale", GetScale());

}

void CompTransform::Load(const JSON_Object* object, std::string name)
{
	float3 position = App->fs->json_array_dotget_float3_string(object, name + "Position");
	float4 rotation = App->fs->json_array_dotget_float4_string(object, name + "Rotation");
	float3 scale = App->fs->json_array_dotget_float3_string(object, name + "Scale");
	Init(position, rotation, scale);
	Enable();
}

