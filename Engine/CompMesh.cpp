#include "Application.h"
#include "Globals.h"
#include "Component.h"
#include "CompMesh.h"
#include "CompMaterial.h"
#include "CompTransform.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ImportMesh.h"
#include "ResourceMesh.h"
#include "Color.h"

#include <vector>


CompMesh::CompMesh(Comp_Type t, GameObject* parent_) : Component(t, parent_)
{
	name = "Mesh";
	uid = App->random->Int();
	parent = parent_;
}

CompMesh::~CompMesh()
{
	RELEASE_ARRAY(name);
	resourceMesh->NumGameObjectsUseMe--;
	material = nullptr;
	resourceMesh = nullptr;
}



//void CompMesh::Init(std::vector<_Vertex> v, std::vector<uint> i)
//{
//	vertices = v;
//	indices = i;
//
//	float3 vert_origin;
//	float3 vert_norm;
//
//	SetupMesh();
//}

void CompMesh::ShowInspectorInfo()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 0));
	ImGui::SameLine(ImGui::GetWindowWidth() - 26);
	if (ImGui::ImageButton((ImTextureID*)App->scene->icon_options_transform, ImVec2(13, 13), ImVec2(-1, 1), ImVec2(0, 0)))
	{
		ImGui::OpenPopup("Options");
	}
	ImGui::PopStyleVar();

	// Button Options --------------------------------------
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.00f));
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
		if (ImGui::Button("Reset Mesh"))
		{
			resourceMesh = nullptr;
			ImGui::CloseCurrentPopup();
		}
		ImGui::Separator();
		if (ImGui::Button("Select Mesh..."))
		{
			SelectMesh = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor();
	if (resourceMesh != nullptr)
	{
		ImGui::Text("Name:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%s", resourceMesh->name);
		ImGui::Text("Vertices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", resourceMesh->num_vertices);
		ImGui::Text("Indices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", resourceMesh->num_indices);

		ImGui::Checkbox("Render", &render);
	}
	else
	{
		if (ImGui::Button("Select Mesh..."))
		{
			SelectMesh = true;
		}
		if (SelectMesh)
		{
			resourceMesh = (ResourceMesh*)App->resource_manager->ShowResources(SelectMesh);
			if (resourceMesh != nullptr)
			{
				if (resourceMesh->isLoaded == false)
				{
					App->importer->iMesh->LoadResource(std::to_string(resourceMesh->uuid_mesh).c_str(), resourceMesh);
				}
				Enable();
				parent->Addbounding_box();
			}
		}
	}
	ImGui::TreePop();
}

void CompMesh::Draw()
{
	if (render && resourceMesh != nullptr)
	{
		CompTransform* transform = (CompTransform*)parent->FindComponentByType(C_TRANSFORM);
		if (transform != nullptr)
		{
			/* Push Matrix to Draw with transform applied, only if it contains a transform component */
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(transform->GetMultMatrixForOpenGL());
		}

		if (resourceMesh->vertices.size() > 0 && resourceMesh->indices.size() > 0)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			//Set Wireframe
			if (App->renderer3D->wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}

			//Set Color
			if (material != nullptr)
			{
				glColor4f(material->GetColor().r, material->GetColor().g, material->GetColor().b, material->GetColor().a);
			}
			else
			{
				glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
			}

			if (App->renderer3D->texture_2d)
			{
				CompMaterial* temp = parent->GetComponentMaterial();
				if(temp != nullptr)
					glBindTexture(GL_TEXTURE_2D, temp->GetTextureID());
			}

			glBindBuffer(GL_ARRAY_BUFFER, resourceMesh->vertices_id); //VERTEX ID
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex), NULL);
			glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, norm));
			glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resourceMesh->indices_id); // INDICES ID
			glDrawElements(GL_TRIANGLES, resourceMesh->indices.size(), GL_UNSIGNED_INT, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// NORMALS ----------------------------------
			if (App->renderer3D->normals && hasNormals)
			{
				glBindBuffer(GL_ARRAY_BUFFER, resourceMesh->vertices_norm_id);
				glVertexPointer(3, GL_FLOAT, sizeof(float3), NULL);
				glDrawArrays(GL_LINES, 0, resourceMesh->vertices.size() * 2);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			//Reset TextureColor
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBindTexture(GL_TEXTURE_2D, 0);

			//Disable Wireframe -> only this object will be wireframed
			if (App->renderer3D->wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		else
		{
			LOG("Cannot draw the mesh");
		}

		if (transform != nullptr)
		{
			glPopMatrix();
		}
	}
}

void CompMesh::Update(float dt)
{
}

void CompMesh::Render(bool render)
{
	this->render = render;
}

bool CompMesh::isRendering() const
{
	return render;
}

void CompMesh::LinkMaterial(CompMaterial* mat)
{
	if (mat != nullptr)
	{
		material = mat;
		LOG("MATERIAL linked to MESH %s", name);
	}
}

void CompMesh::SetResource(ResourceMesh* resourse_mesh)
{
	if (resourceMesh != resourse_mesh)
	{
		resourceMesh->NumGameObjectsUseMe--;
		resourceMesh = resourse_mesh;
		resourceMesh->NumGameObjectsUseMe++;
	}
}

void CompMesh::Save(JSON_Object* object, std::string name) const
{
	json_object_dotset_number_with_std(object, name + "Type", C_MESH);
	json_object_dotset_number_with_std(object, name + "UUID", uid);
	json_object_dotset_boolean_with_std(object, name + "Primitive", isPrimitive);
	if (isPrimitive)
	{
		json_object_dotset_number_with_std(object, name + "Type primitive", TypePrimitive);
	}
	else
	{
		json_object_dotset_number_with_std(object, name + "Resource Mesh ID", resourceMesh->GetUUID());
	}
}

void CompMesh::Load(const JSON_Object* object, std::string name)
{
	isPrimitive = json_object_dotget_boolean_with_std(object, name + "Primitive");
	uid = json_object_dotget_number_with_std(object, name + "UUID");
	if (isPrimitive)
	{
		TypePrimitive = json_object_dotget_number_with_std(object, name + "Type primitive");
		if (TypePrimitive == 1 /*TYPE_SPHERE*/)
		{
			//float3* vertices_array = new float3[1536];

			//Sphere* sphere = new Sphere(float3::zero, 1);
			//sphere->Triangulate(vertices_array, NULL, NULL, 1536, false);

			//App->scene->Init_IndexVertex(vertices_array, 1536, this);
			//InitRanges(vertices.size(), indices.size(), 0);
			//SetupMesh();
			//Enable();

			///* Set Bounding Box */
			//parent->bounding_box = new AABB();
			//parent->bounding_box->SetNegativeInfinity();
			//parent->bounding_box->Enclose(vertices, num_vertices);
		}
		if (TypePrimitive == 2/*TYPE_CUBE*/)
		{
			//InitRanges(8, 36, 0); // 0 normals by now.

			//OBB* box = new OBB();
			//box->pos = float3::zero;
			//box->r = float3::one;
			//box->axis[0] = float3(1, 0, 0);
			//box->axis[1] = float3(0, 1, 0);
			//box->axis[2] = float3(0, 0, 1);

			//parent->bounding_box = new AABB(*box);
			//float3* vertices_array = new float3[36];

			//parent->bounding_box->Triangulate(1, 1, 1, vertices_array, NULL, NULL, false);

			//App->scene->Init_IndexVertex(vertices_array, num_indices, this);
			//SetupMesh();
			//Enable();
		}
	}
	else
	{
		uint resourceID = json_object_dotget_number_with_std(object, name + "Resource Mesh ID");
		resourceMesh = (ResourceMesh*)App->resource_manager->GetResource(resourceID);
		resourceMesh->NumGameObjectsUseMe++;
		//TODO ELLIOT -> LOAD MESH
		//const char* directory = App->GetCharfromConstChar(std::to_string(uuid_mesh).c_str());
		if (resourceMesh->isLoaded == false)
		{
			App->importer->iMesh->LoadResource(std::to_string(resourceMesh->uuid_mesh).c_str(), resourceMesh);
		}

		Enable();
	}
}
