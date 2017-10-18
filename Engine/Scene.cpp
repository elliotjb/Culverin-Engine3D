#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "Primitive.h"
#include "ModuleLoader.h"
#include "Gl3W\include\glew.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "ModuleGeometries.h"
#include "_Cube.h"
#include "GameObject.h"
#include <direct.h>

Scene::Scene(bool start_enabled) : Module(start_enabled)
{
	Start_enabled = true;
	preUpdate_enabled = true;
	Update_enabled = true;

	name = "Scene";
}

Scene::~Scene()
{
}

bool Scene::Start()
{
	perf_timer.Start();
	
	Start_t = perf_timer.ReadMs();

	return true;
}

update_status Scene::PreUpdate(float dt)
{
	perf_timer.Start();

	frBuff->Bind();

	preUpdate_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

update_status Scene::Update(float dt)
{
	perf_timer.Start();

	//Draw Plane
	DrawPlane(7);

	//if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	//{
	//	mkdir("NewFolderElliot");
	//}
	//FILE* temp; 
	//temp = fopen("elliotjb", "r");
	
	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	return true;
}


void Scene::DrawPlane(int size)
{
	glBegin(GL_LINES);
	for (int i = -size; i <= size; i++)
	{
		glVertex3f(-size, 0, i);
		glVertex3f(size, 0, i);
	}
	for (int i = -size; i <= size; i++)
	{
		glVertex3f(i, 0, size);
		glVertex3f(i, 0, -size);
	}

	glEnd();
}


GameObject* Scene::CreateGameObject()
{
	GameObject* obj = new GameObject();
	rootObject = obj;
	return obj;
}

/*for (int i = 0; i < CHECKERS_HEIGHT; i++)
{
for (int j = 0; j < CHECKERS_WIDTH; j++)
{
int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
checkImage[i][j][0] = (GLubyte)c;
checkImage[i][j][1] = (GLubyte)c;
checkImage[i][j][2] = (GLubyte)c;
checkImage[i][j][3] = (GLubyte)255;
}
}*/