#include "CompTransform.h"
#include "Component.h"

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

void CompTransform::ShowInspectorInfo()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PopStyleColor();
		int op = ImGui::GetWindowWidth() / 4;
		ImGui::PushItemWidth(op - 40);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 1));
		ImGui::Text("Position"); ImGui::SameLine(op + 40);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_1", &position.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_1", &position.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_1", &position.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Rotation"); ImGui::SameLine(op + 40);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_2", &rotation.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_2", &rotation.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_2", &rotation.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Scale"); ImGui::SameLine(op + 40);
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##X_3", &scale.x, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##Y_3", &scale.y, 0, 0, 3, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##Z_3", &scale.z, 0, 0, 3, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
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
}

void CompTransform::SetRot(float3 rot)
{
	rotation = rot;
}

void CompTransform::SetScale(float3 scal)
{
	scale = scal;
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

