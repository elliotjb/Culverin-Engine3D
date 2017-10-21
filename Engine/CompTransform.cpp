#include "CompTransform.h"
#include "Component.h"

#include <gl/GL.h>
#include <gl/GLU.h>

CompTransform::CompTransform(Comp_Type t) :Component(t)
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
	glMultMatrixf((float*)&matrix.Transposed());
}

void CompTransform::ShowInspectorInfo()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PopStyleColor();
		
		/*int op = ImGui::GetWindowWidth() / 4;
		ImGui::PushItemWidth(op - 40);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 1));*/

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

		//ImGui::PopStyleVar();
		//ImGui::PopItemWidth();
		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}
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
	matrix = float4x4::FromTRS(position, rot_quat, scale);
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

