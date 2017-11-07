#include "CompTransform.h"
#include "Component.h"
#include "GameObject.h"
#include "ImGui/ImGuizmo.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "CompCamera.h"

#include <gl/GL.h>
#include <gl/GLU.h>

CompTransform::CompTransform(Comp_Type t, GameObject* parent) :Component(t, parent)
{
}

CompTransform::~CompTransform()
{
}

void CompTransform::Init(float3 p, float3 r, float3 s)
{
	SetPos(p);
	SetRot(r);
	SetScale(s);
}

void CompTransform::Update()
{
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

		// Button Reset Values
		static GLuint icon_options_transform = App->textures->LoadTexture("Images/UI/icon_options_transform.png");
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 0));
		ImGui::SameLine(ImGui::GetWindowWidth() - 26);
		static float3 rot = rotation;
		if (ImGui::ImageButton((ImTextureID*)icon_options_transform, ImVec2(13, 13), ImVec2(-1, 1), ImVec2(0, 0)))
		{
			ImGui::OpenPopup("Options");
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		// Button Options --------------------------------------
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.00f));
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
			if (ImGui::Button("Reset Values"))
			{
				position = math::float3(0, 0, 0);
				//SetRot(math::float3(-rot.x/2, -rot.y/2, -rot.z/2));
				//rot = math::float3(0, 0, 0);
				scale = math::float3(1, 1, 1);
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			if (ImGui::Button("Reset Position"))
			{
				position = math::float3(0, 0, 0);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Reset Rotation"))
			{
				//SetRot(math::float3(-rot.x/2, -rot.y/2, -rot.z/2));
				//rot = math::float3(0, 0, 0);
				ImGui::CloseCurrentPopup();
			} ImGui::SameLine(); App->ShowHelpMarker("Doesn't Work!!");
			if (ImGui::Button("Reset Size"))
			{
				scale = math::float3(1, 1, 1);
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleVar();
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor();
		
		// Values: Position, Rotation, Scale -------------------------------
		ImGui::Spacing();

		// GIZMO TEST -----------------------------------------------------
		/*static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

		if (ImGui::IsKeyPressed(78)) // N Key 
		{
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		}
		if (ImGui::IsKeyPressed(69)) // E Key
		{
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		}
		if (ImGui::IsKeyPressed(82)) // R Key
		{
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		}

		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		{
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		}
		ImGui::SameLine();

		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		{
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		}			
		ImGui::SameLine();

		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		{
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		}

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(local_transform.ptr(), matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation, 3);
		ImGui::InputFloat3("Rt", matrixRotation, 3);
		ImGui::InputFloat3("Sc", matrixScale, 3);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, local_transform.ptr());
		
		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			{
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			}				
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			{
				mCurrentGizmoMode = ImGuizmo::WORLD;
			}
		}

		//static bool useSnap(false);

		//if (ImGui::IsKeyPressed(83)) // S Key
		//{
		//	useSnap = !useSnap;
		//}
		//ImGui::Checkbox("", &useSnap);
		//ImGui::SameLine();

		//float4 snap;

		//switch (mCurrentGizmoOperation)
		//{
		//case ImGuizmo::TRANSLATE:
		//	snap = config.mSnapTranslation;
		//	ImGui::InputFloat3("Snap", &snap.x);
		//	break;
		//case ImGuizmo::ROTATE:
		//	snap = config.mSnapRotation;
		//	ImGui::InputFloat("Angle Snap", &snap.x);
		//	break;
		//case ImGuizmo::SCALE:
		//	snap = config.mSnapScale;
		//	ImGui::InputFloat("Scale Snap", &snap.x);
		//	break;
		//}

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);*/
		//ImGuizmo::Manipulate(App->camera->GetViewMatrix(), App->camera->GetProjMatrix(), mCurrentGizmoOperation, mCurrentGizmoMode, local_transform.ptr(), NULL, NULL);
		
		// ----------------------------------------------------------------

		// ORIGINAL -------------------------------------------------------
		int op = ImGui::GetWindowWidth() / 4;
		ImGui::Text("Position"); ImGui::SameLine(op + 30);
		bool isMoveMouse = false;
		if (ImGui::DragFloat3("##pos", &position[0], 0.5f))
		{
			isMoveMouse = true;
			SetPos(position);
		}
		ImGui::Text("Rotation"); ImGui::SameLine(op + 30);
		if (ImGui::DragFloat3("##rot", &rot[0], 0.5f))
		{
			isMoveMouse = true;
			SetRot(rot);
		}
		ImGui::Text("Scale"); ImGui::SameLine(op + 30);
		if (ImGui::DragFloat3("##scal", &scale[0], 0.5f))
		{
			isMoveMouse = true;
			SetScale(scale);
		}
		// ------------------------------------------------------------------

		// This function let mouse trespassing the screen to enter from the opposite side
		if (isMoveMouse)
		{
			if (App->input->GetMouseX() <= width &&
				App->input->GetMouseX() > width - 10)
			{
				SetCursorPos(30, App->input->GetMouseY());
			}
			if (App->input->GetMouseX() >= 0 &&
				App->input->GetMouseX() < 10)
			{
				SetCursorPos(width - 20, App->input->GetMouseY());
			}
		}
		// -------------------------------------------------------------------------------

		//static bool useSnap(false);
		//if (ImGui::IsKeyPressed(83))
		//	useSnap = !useSnap;
		//ImGui::Checkbox("", &useSnap);

		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}


	//ImGuizmo::RecomposeMatrixFromComponents((float*)&position, (float*)&rotation, (float*)&scale,(float*)&local_transform);
	//float4 snap;
	//switch (mCurrentGizmoOperation)
	//{
	//case ImGuizmo::TRANSLATE:
	//	snap = config.mSnapTranslation;
	//	ImGui::InputFloat3("Snap", &snap.x);
	//	break;
	//case ImGuizmo::ROTATE:
	//	snap = config.mSnapRotation;
	//	ImGui::InputFloat("Angle Snap", &snap.x);
	//	break;
	//case ImGuizmo::SCALE:
	//	snap = config.mSnapScale;
	//	ImGui::InputFloat("Scale Snap", &snap.x);
	//	break;
	//}

	//ImGuiIO& io = ImGui::GetIO();
	//ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	//ImGuizmo::Manipulate((float*)&App->renderer3D->ViewMatrix, (float*)&App->renderer3D->ProjectionMatrix, mCurrentGizmoOperation, mCurrentGizmoMode, (float*)&local_transform, NULL /*, useSnap ? &snap.x : NULL*/);

}

void CompTransform::SetPos(float3 pos)
{
	position = pos;

	UpdateMatrix();
}

void CompTransform::SetRot(float3 rot)
{
	rot_angle = (rot - rotation) * DEGTORAD;
	rot_quat = rot_quat * Quat::FromEulerXYZ(rot_angle.x, rot_angle.y, rot_angle.z);
	rotation = rot;

	UpdateMatrix();
}

void CompTransform::SetScale(float3 scal)
{
	scale = scal;

	UpdateMatrix();
}

void CompTransform::SetZero()
{
	SetPos(float3::zero);
	SetRot(float3::zero);
	SetScale(float3(1, 1, 1));
}

void CompTransform::SetTransformation(aiMatrix4x4 transform)
{
	//TRANSFORM DATA ---------------------------
	aiQuaternion rot_quat;
	aiVector3D pos, rot, scal;
	float3 pos_vec, rot_vec, scal_vec;

	transform.Decompose(scal, rot_quat, pos);
	rot = rot_quat.GetEuler();

	pos_vec.Set(pos.x, pos.y, pos.z);
	rot_vec.Set(rot.x, rot.y, rot.z);
	scal_vec.Set(scal.x, scal.y, scal.z);
	SetPos(pos_vec);
	SetRot(rot_vec);
	SetScale(scal_vec);
	//------------------------------------------
}

void CompTransform::SetLocalTransform()
{
	local_transform = float4x4::FromTRS(position, rot_quat, scale);;
}

void CompTransform::SetGlobalTransform(float4x4 transform)
{
	global_transform = transform;
}

void CompTransform::UpdateMatrix()
{
	float4x4 temp = global_transform;
	global_transform = float4x4::FromTRS(position, rot_quat, scale);
	local_transform = float4x4::FromTRS(position, rot_quat, scale);
	temp = temp - global_transform;

	parent->UpdateMatrixRecursive(temp, false);
}

float3 CompTransform::GetPos() const
{
	return position;
}

float3 CompTransform::GetRot() const
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

const float* CompTransform::GetMultMatrixForOpenGL() const
{
	return global_transform.Transposed().ptr(); //Change Matrix to Global Matrix
}


void CompTransform::Save(JSON_Object* object, std::string name) const
{
	// TRANSFORM-----------
	json_object_dotset_number_with_std(object, name + "Type", C_TRANSFORM);
	// Position
	App->fs->json_array_dotset_float3(object, name + "Position", GetPos());
	// Rotation
	App->fs->json_array_dotset_float3(object, name + "Rotation", GetRot());
	// Scale
	App->fs->json_array_dotset_float3(object, name + "Scale", GetScale());

}

void CompTransform::Load(const JSON_Object* object, std::string name)
{
	float3 position = App->fs->json_array_dotget_float3_string(object, name + "Position");
	float3 rotation = App->fs->json_array_dotget_float3_string(object, name + "Rotation");
	float3 scale = App->fs->json_array_dotget_float3_string(object, name + "Scale");
	Init(position, rotation, scale);
	Enable();
}

