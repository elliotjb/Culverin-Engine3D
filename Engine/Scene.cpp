#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "Primitive.h"
#include "ModuleFBXLoader.h"
#include "Gl3W\include\glew.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "ModuleObjects.h"
#include "_Cube.h"

Scene::Scene(bool start_enabled) : Module(start_enabled)
{
	name = "Scene";
}

Scene::~Scene()
{
}

bool Scene::Start()
{
	float3 pos = { 1, 1, 1 };
	float3 size = { 1, 1, 1 };
	//App->objects->CreateCube(pos, size);
	
	for (int i = 0; i < CHECKERS_HEIGHT; i++)
	{
		for (int j = 0; j < CHECKERS_WIDTH; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);


	return true;
}

update_status Scene::Update(float dt)
{
	perf_timer.Start();

	Plane_p p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, ImageName);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	
	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 1, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 1, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 1, -1);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 1, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 1, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 1, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1, -1);

	//----
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1, 0);

	//----
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 1, -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 1, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1, -1);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 0, 0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glLineWidth(1.0f); 

	Update_t = perf_timer.ReadMs();

	return UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	return true;
}

void Scene::ShowPerformance(int ms_index)
{
	if (ImGui::CollapsingHeader("SCENE"))
	{
		ImGui::Text("Pre-Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.4f", pre_log[ms_index - 1]);
		ImGui::PlotHistogram("", pre_log, IM_ARRAYSIZE(pre_log), 0, NULL, 0.0f, 5.0f, ImVec2(0, 80));
		ImGui::Text("Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.4f", up_log[ms_index - 1]);
		ImGui::PlotHistogram("", up_log, IM_ARRAYSIZE(up_log), 0, NULL, 0.0f, 50.0f, ImVec2(0, 80));
		ImGui::Text("Post-Update:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.4f", post_log[ms_index - 1]);
		ImGui::PlotHistogram("", post_log, IM_ARRAYSIZE(post_log), 0, NULL, 0.0f, 50.0f, ImVec2(0, 80));
	}
}
