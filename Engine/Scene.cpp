#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Component.h"
#include "CompTransform.h"
#include "CompMesh.h"
#include "CompMaterial.h"
#include "WindowInspector.h"
#include "CompCamera.h"
#include "MathGeoLib.h"
#include "Quadtree.h"
#include "JSONSerialization.h"

#include "Gl3W/include/glew.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"

#include <direct.h>


#define SPHERE_DEFINITION 1536

Scene::Scene(bool start_enabled) : Module(start_enabled)
{
	Start_enabled = true;
	preUpdate_enabled = true;
	Update_enabled = true;

	name = "Scene";
	haveConfig = true;
}

Scene::~Scene()
{
}

bool Scene::Start()
{
	perf_timer.Start();

	//Init Quadtree
	quadtree.Boundaries(AABB(float3(-50.0f, -50.0f, -50.0f), float3(50.0f, 50.0f, 50.0f)));

	size_plane = 50;

	CreateMainCamera(nullptr);

	//Init Skybox
	InitSkybox();

	Start_t = perf_timer.ReadMs();

	return true;
}

update_status Scene::PreUpdate(float dt)
{
	perf_timer.Start();

	// PreUpdate GameObjects ------------------------
	for (uint i = 0; i < gameobjects.size(); i++)
	{
		gameobjects[i]->preUpdate(dt);
	}

	preUpdate_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

update_status Scene::Update(float dt)
{
	perf_timer.Start();

	// Draw Ray ----------------------
	//glBegin(GL_LINES);

	//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//glVertex3f(App->camera->ray.a.x, App->camera->ray.a.y, App->camera->ray.a.z); glVertex3f(App->camera->ray.b.x, App->camera->ray.b.y, App->camera->ray.b.z);

	//glEnd();
	// -----------------------------

	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		//gameobjects[0]->AddChildGameObject_Copy(gameobjects[1]);
		//gameobjects[0]->AddChildGameObject(gameobjects[2]);
		//SaveScene();
		//App->Json_seria->LoadPrefab("Assets/BakerHouse.fbx.meta.json");
	}

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		//for (int i = gameobjects.size() - 1; i >= 0; i--)
		//{
		//	delete gameobjects[i];
		//	gameobjects.pop_back();
		//}
		//((Inspector*)App->gui->winManager[INSPECTOR])->SetLinkObjectNull();
		
		App->Json_seria->LoadScene();
	}

	// Update GameObjects (Only in Game Time) -----------
	for (uint i = 0; i < gameobjects.size(); i++)
	{
		gameobjects[i]->Update(dt);
	}
	// -------------------------------------------------

	//if (quadtree_draw)
	//{
	//	quadtree.DebugDraw();
	//}

	
	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

update_status Scene::UpdateConfig(float dt)
{
	ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4);
	ImGui::SliderInt("Plane Size", &size_plane, 5, 1000);

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 0.761f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("QUADTREE"))
	{
		ImGui::PopStyleColor();

		ImGui::Checkbox("##quadtreedraw", &quadtree_draw); ImGui::SameLine();
		ImGui::Text("Draw Quadtree");

		if(ImGui::Button("UPDATE QUADTREE"))	
		{ 
			quadtree.Bake(App->scene->gameobjects);
		}

		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}


	ImGui::PopItemWidth();
	return UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	DeleteSkyboxTex();
	return true;
}


void Scene::DrawPlane()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBegin(GL_LINES);
	for (int i = -size_plane; i <= size_plane; i++)
	{
		glVertex3f(-size_plane, 0, i);
		glVertex3f(size_plane, 0, i);
	}
	for (int i = -size_plane; i <= size_plane; i++)
	{
		glVertex3f(i, 0, size_plane);
		glVertex3f(i, 0, -size_plane);
	}

	glEnd();
}

void Scene::DrawCube(float size)
{

	float difamb[] = { 1.0f, 0.5f, 0.3f, 1.0f };
	glBegin(GL_QUADS);
	//front face
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, difamb);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);

	//left face
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);

	//back face
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);

	//right face
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);

	//top face
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glVertex3f(size / 2, size / 2, -size / 2);

	//bottom face
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();

}

void Scene::InitSkybox()
{
	skybox[SkyBoxSide::SKY_LEFT] = App->textures->LoadSkyboxTexture("Images/Skybox/left.png");
	skybox[SkyBoxSide::SKY_BACK] = App->textures->LoadSkyboxTexture("Images/Skybox/back.png");
	skybox[SkyBoxSide::SKY_RIGHT] = App->textures->LoadSkyboxTexture("Images/Skybox/right.png");
	skybox[SkyBoxSide::SKY_FRONT] = App->textures->LoadSkyboxTexture("Images/Skybox/front.png");
	skybox[SkyBoxSide::SKY_TOP] = App->textures->LoadSkyboxTexture("Images/Skybox/top.png");
	skybox[SkyBoxSide::SKY_BOTTOM] = App->textures->LoadSkyboxTexture("Images/Skybox/bottom.png");
}

void Scene::DrawSkybox(float size, float3 pos)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//back face
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_BACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(1, 0); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 1); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glEnd();

	//left face
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_LEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(1, 0); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(0, 1); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glEnd();

	//front face
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_FRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(0, 0); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(0, 1); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glEnd();

	//right face
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_RIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 0); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 1); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glEnd();

	//top face
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_TOP]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 0); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 1); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glEnd();

	//bottom face
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_BOTTOM]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 1); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 0); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 0); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glEnd();

	if (App->renderer3D->lighting)
	{
		glEnable(GL_LIGHTING);
	}
	if (App->renderer3D->depth_test)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (!App->renderer3D->texture_2d)
	{
		glDisable(GL_TEXTURE_2D);
	}
}


void Scene::DeleteSkyboxTex()
{
	glDeleteTextures(6, &skybox[0]);
}


void Scene::Init_IndexVertex(float3 * vertex_triangulate, uint num_index, CompMesh * mesh)
{
	std::vector<float3> all_index;
	uint size = 0;
	bool temp = false;

	for (int i = 0; i < num_index; i++)
	{
		temp = false;
		size = all_index.size();

		for (int j = 0; j < size; j++)
		{
			if (all_index[j] == vertex_triangulate[i])
			{
				mesh->indices.push_back(j);
				temp = true;
			}
		}

		if (temp == false)
		{
			_Vertex vertex;
			all_index.push_back(vertex_triangulate[i]);
			mesh->indices.push_back(all_index.size() - 1);
			vertex.pos = vertex_triangulate[i];
			mesh->vertices.push_back(vertex);
		}
	}
}

GameObject* Scene::CreateGameObject(GameObject* parent)
{
	GameObject* obj = new GameObject(parent);
	if (parent != nullptr)
	{
		gameobjects.push_back(obj);
	}
	return obj;
}

void Scene::DeleteGameObjects(std::vector<GameObject*>& gameobjects)
{
	for (int i = 0; i < gameobjects.size(); i++)
	{
		if (gameobjects[i]->GetNumChilds() > 0)
		{
			DeleteGameObjects(gameobjects[i]->GetChildsVec());
		}
		else
		{
			// First delete all components


			// Now Delete GameObject
			//delete gameobjects[i];
			gameobjects[i]->GetChildsVec().clear();
		}
	}
	gameobjects.clear();
}

GameObject* Scene::CreateCube(GameObject* parent)
{
	GameObject* obj = new GameObject(parent);

	// SET NAME -----------------------------------
	static uint cube_count = 0;
	std::string name = "Cube ";
	name += std::to_string(cube_count++);
	char* name_str = new char[name.size()+1];
	strcpy(name_str, name.c_str());
	obj->SetName(name_str);

	/* Predefined Cube has 3 Base components: Transform, Mesh & Material */

	//TRANSFORM COMPONENT --------------
	CompTransform* transform = (CompTransform*)obj->AddComponent(C_TRANSFORM);
	transform->Init(float3(0, 0, 0), float3(0, 0, 0), float3(1, 1, 1));
	transform->Enable();

	//MESH COMPONENT -------------------
	CompMesh* mesh = (CompMesh*)obj->AddComponent(C_MESH);
	mesh->isPrimitive = true;
	mesh->TypePrimitive = 2;
	mesh->InitRanges(8, 36, 0); // 0 normals by now.

	OBB* box = new OBB();
	box->pos = float3::zero;
	box->r = float3::one;
	box->axis[0] = float3(1, 0, 0);
	box->axis[1] = float3(0, 1, 0);
	box->axis[2] = float3(0, 0, 1);

	obj->bounding_box = new AABB(*box);
	float3* vertices_array = new float3[36];

	obj->bounding_box->Triangulate(1, 1, 1, vertices_array, NULL, NULL, false);

	Init_IndexVertex(vertices_array, mesh->num_indices, mesh);
	mesh->SetupMesh();
	mesh->Enable();

	//MATERIAL COMPONENT -------------------
	CompMaterial* mat = (CompMaterial*)obj->AddComponent(C_MATERIAL);
	mat->Enable();

	if (parent == nullptr)
	{
		// Only add to GameObjects list the Root Game Objects
		App->scene->gameobjects.push_back(obj);
	}

	LOG("CUBE Created.");

	return obj;
}

GameObject* Scene::CreateSphere(GameObject* parent)
{
	GameObject* obj = new GameObject(parent);

	// SET NAME ----------------------------------
	static uint sphere_count = 0;
	std::string name = "Sphere ";
	name += std::to_string(sphere_count++);
	char* name_str = new char[name.size() + 1];
	strcpy(name_str, name.c_str());
	obj->SetName(name_str);

	/*Predefined sPHERE has 2 Base components: Transform, Mesh & Material */

	// TRANSFORM COMPONENT --------------
	CompTransform* transform = (CompTransform*)obj->AddComponent(C_TRANSFORM);
	transform->Init(float3(0, 0, 0), float3(0, 0, 0), float3(1, 1, 1));
	transform->Enable();

	// MESH COMPONENT -------------------
	CompMesh* mesh = (CompMesh*)obj->AddComponent(C_MESH);
	mesh->isPrimitive = true;
	mesh->TypePrimitive = 1;

	float3* vertices_array = new float3[SPHERE_DEFINITION];

	Sphere* sphere = new Sphere(float3::zero, 1);
	sphere->Triangulate(vertices_array, NULL, NULL, SPHERE_DEFINITION, false);
	
	Init_IndexVertex(vertices_array, SPHERE_DEFINITION, mesh);
	mesh->InitRanges(mesh->vertices.size(), mesh->indices.size(), 0);
	mesh->SetupMesh();
	mesh->Enable();

	/* Set Bounding Box */
	obj->bounding_box = new AABB();
	obj->bounding_box->SetNegativeInfinity();
	obj->bounding_box->Enclose(mesh->vertices, mesh->num_vertices);

	// MATERIAL COMPONENT -------------------
	CompMaterial* mat = (CompMaterial*)obj->AddComponent(C_MATERIAL);
	mat->Enable();

	if (parent == nullptr)
	{
		// Only add to GameObjects list the Root Game Objects
		App->scene->gameobjects.push_back(obj);
	}

	LOG("SPHERE Created.");

	return obj;
}

GameObject* Scene::CreateMainCamera(GameObject* parent)
{
	GameObject* obj = new GameObject(parent);

	// SET NAME -----------------------------------
	std::string name = "MainCamera";
	char* name_str = new char[name.size() + 1];
	strcpy(name_str, name.c_str());
	obj->SetName(name_str);

	/* Predefined Main Camera has 2 Base components: Transform & Camera */

	// TRANSFORM COMPONENT --------------
	CompTransform* transform = (CompTransform*)obj->AddComponent(C_TRANSFORM);
	transform->Init(float3(0, 0, 0), float3(0, 0, 0), float3(1, 1, 1));
	transform->Enable();

	// CAMERA COMPONENT -----------------
	CompCamera* camera = (CompCamera*)obj->AddComponent(C_CAMERA);
	camera->Enable();
	camera->SetMain(true);

	if (parent == nullptr)
	{
		// Only add to GameObjects list the Root Game Objects
		App->scene->gameobjects.push_back(obj);
	}

	LOG("MAIN CAMERA Created.");

	return obj;
}