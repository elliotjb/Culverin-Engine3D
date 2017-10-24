#include "CompTransform.h"
#include "Component.h"
#include "GameObject.h"
#include "ImGui/ImGuizmo.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

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
	//glMultMatrixf((float*)&local_transform.Transposed());
}

void CompTransform::ShowInspectorInfo()
{
	//static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	//static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	//if (ImGui::IsKeyPressed(90))
	//	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	//if (ImGui::IsKeyPressed(69))
	//	mCurrentGizmoOperation = ImGuizmo::ROTATE;
	//if (ImGui::IsKeyPressed(82)) // r Key
	//	mCurrentGizmoOperation = ImGuizmo::SCALE;
	//if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
	//	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PopStyleColor();
		
		ImGui::Text("Position"); 
		if (ImGui::DragFloat3("##pos", &position[0], 0.5f))
		{
			SetPos(position);
		}
		static float3 rot = rotation;
		ImGui::Text("Rotation");
		if (ImGui::DragFloat3("##rot", &rot[0], 0.5f))
		{
			SetRot(rot);
		}
		ImGui::Text("Scale");
		if (ImGui::DragFloat3("##scal", &scale[0], 0.5f))
		{
			SetScale(scale);
		}

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

void CompTransform::UpdateMatrix()
{
	local_transform = float4x4::FromTRS(position, rot_quat, scale);
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

float4x4 CompTransform::GetTransform() const
{
	return local_transform;
}

float4x4 CompTransform::GetInheritedTransform() const
{
	return inherited_transform;
}

const float* CompTransform::GetMultMatrixForOpenGL() const
{
	return local_transform.Transposed().ptr(); //Change Matrix to Global Matrix
}

