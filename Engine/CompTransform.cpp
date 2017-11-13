#include "CompTransform.h"
#include "Component.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "CompCamera.h"
#include "WindowInspector.h"
#include "WindowSceneWorld.h"
#include "ImGui/ImGuizmo.h"
#include <gl/GL.h>
#include <gl/GLU.h>

CompTransform::CompTransform(Comp_Type t, GameObject* parent) :Component(t, parent)
{
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
	if (((Inspector*)App->gui->winManager[INSPECTOR])->GetSelected() == parent)
	{
		ImGuizmo::Enable(true);

		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

		screen = ((SceneWorld*)App->gui->winManager[SCENEWORLD])->GetWindowParams();
		ImGuizmo::SetRect(screen.x, screen.y, screen.z, screen.w);

		local_transposed = local_transform.Transposed();

		// SET GUIZMO OPERATION ----------------------------------
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		}
		else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		}
		else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		}

		ImGuizmo::Manipulate(App->camera->GetViewMatrix(), App->camera->GetProjMatrix(), mCurrentGizmoOperation, transform_mode, local_transposed.ptr(), NULL, NULL);
		
		if (ImGuizmo::IsUsing())
		{
			local_transposed.Transpose();
			local_transposed.Decompose(position, rotation, scale);
			rotation_euler = rotation.ToEulerXYZ() * RADTODEG;
			toUpdate = true;
		}
	}

	if (toUpdate)
	{
		UpdateMatrix(transform_mode);
		toUpdate = false;
	}
}

void CompTransform::ShowInspectorInfo()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Set Size Windows
		static int width;
		static int height;
		SDL_GetWindowSize(App->window->window, &width, &height);

		// Reset Values Button -------------------------------------------
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 0));
		ImGui::SameLine(ImGui::GetWindowWidth() - 26);
		if (ImGui::ImageButton((ImTextureID*)App->scene->icon_options_transform, ImVec2(13, 13), ImVec2(-1, 1), ImVec2(0, 0)))
		{
			ImGui::OpenPopup("Options");
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		// Options Button --------------------------------------
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.00f));
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
			if (ImGui::Button("Reset Values"))
			{
				ResetMatrix();
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			if (ImGui::Button("Reset Position"))
			{
				SetPos(float3::zero);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Reset Rotation"))
			{
				SetRot(math::float3::zero);
				rotation_euler = float3::zero;
				ImGui::CloseCurrentPopup();
			} ImGui::SameLine(); App->ShowHelpMarker("Doesn't Work!!");
			if (ImGui::Button("Reset Size"))
			{
				SetScale(math::float3(1, 1, 1));
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleVar();
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor();
		
		ImGui::Spacing();

		// ORIGINAL -------------------------------------------------------
		if (ImGui::RadioButton("Local", transform_mode == ImGuizmo::LOCAL))
		{
			transform_mode = ImGuizmo::LOCAL;
		}		
		ImGui::SameLine();
		if (ImGui::RadioButton("World", transform_mode == ImGuizmo::WORLD))
		{
			transform_mode = ImGuizmo::WORLD;
		}
		int op = ImGui::GetWindowWidth() / 4;

		switch (transform_mode)
		{
		case (ImGuizmo::MODE::LOCAL):
		{
			ImGui::Text("Position"); ImGui::SameLine(op + 30);
			if (ImGui::DragFloat3("##pos", &position[0], 0.5f))
			{
				SetPos(position);
			}
			ImGui::Text("Rotation"); ImGui::SameLine(op + 30);
			if (ImGui::DragFloat3("##rot", &rotation_euler[0], 0.5f))
			{
				SetRot(rotation_euler);
			}
			break;
		}
		case (ImGuizmo::MODE::WORLD):
		{
			ImGui::Text("Position"); ImGui::SameLine(op + 30);
			if (ImGui::DragFloat3("##pos", &position_global[0], 0.5f))
			{
				SetPosGlobal(position_global);
			}
			ImGui::Text("Rotation"); ImGui::SameLine(op + 30);
			if (ImGui::DragFloat3("##rot", &rotation_euler_global[0], 0.5f))
			{
				SetRotGlobal(rotation_euler_global);
			}
			break;
		}
		default:
			break;
		}
		ImGui::Text("Scale"); ImGui::SameLine(op + 30);
		if (ImGui::DragFloat3("##scal", &scale[0], 0.5f))
		{
			SetScale(scale);
		}
		// ------------------------------------------------------------------

		// This function let mouse trespassing the screen to enter from the opposite side
		//if (isMoveMouse)
		//{
		//	if (App->input->GetMouseX() <= width &&
		//		App->input->GetMouseX() > width - 10)
		//	{
		//		SetCursorPos(30, App->input->GetMouseY());
		//	}
		//	if (App->input->GetMouseX() >= 0 &&
		//		App->input->GetMouseX() < 10)
		//	{
		//		SetCursorPos(width - 20, App->input->GetMouseY());
		//	}
		//}
		// -------------------------------------------------------------------------------

		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
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
	std::list<const GameObject*> parents;
	const GameObject* parent_object = parent;

	// Put all parents of the game object to pass from local to global matrix
	while (parent_object != nullptr)
	{
		parents.push_back(parent_object);
		parent_object = parent_object->GetParent();
	}

	std::list<const GameObject*>::reverse_iterator item = parents.rbegin();

	// Multiply parents local transforms (inverted) to get the local transform of this game object
	while (item != parents.crend())
	{
		float4x4 matrix = (*item)->GetComponentTransform()->GetLocalTransform();
		local_transform = local_transform * matrix.Inverted();
		item++;
	}

	//Set Output Variables
	local_transform.Decompose(position_global, rotation_global, scale);
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

	//Set Output Variables
	global_transform.Decompose(position_global, rotation_global, scale);
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
		SetLocalTransform();
		UpdateGlobalTransform();
		break;
	}
	case (ImGuizmo::MODE::WORLD):
	{
		//SetGlobalTransform();
		//UpdateLocalTransform();
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

const float* CompTransform::GetMultMatrixForOpenGL() const
{
	return global_transform.Transposed().ptr();
}

void CompTransform::Save(JSON_Object* object, std::string name) const
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

