#pragma once
#include "CompMaterial.h"
#include "Color.h"

CompMaterial::CompMaterial(Comp_Type t): Component(t)
{
	color = White;
}

CompMaterial::~CompMaterial()
{
}

void CompMaterial::SetColor(float r, float g, float b, float a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

Color CompMaterial::GetColor() const
{
	return color;
}

void CompMaterial::ShowInspectorInfo()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PopStyleColor();

		ImGui::ColorEdit3("", (float*)&color);

		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}
}
