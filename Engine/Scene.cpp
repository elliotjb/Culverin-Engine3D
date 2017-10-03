#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "Primitive.h"
#include "ModuleFBXLoader.h"
#include "Gl3W\include\glew.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"

Scene::Scene(bool start_enabled) : Module(start_enabled)
{
}

Scene::~Scene()
{
}

bool Scene::Start()
{


	return true;
}

update_status Scene::Update(float dt)
{
	perf_timer.Start();

	Plane_p p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	//glLineWidth(2.0f);
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Sphere* sphe = new Sphere(float3(0, 0, 0), 1.0f);

	float3 array_v1[1536];
	float3 array_v2[1536];
	sphe->Triangulate(array_v1, array_v2, NULL, 1536, false);

	GLuint ver;
	glGenBuffers(1, &ver);
	glBindBuffer(GL_ARRAY_BUFFER, ver);
	glBufferData(GL_ARRAY_BUFFER, sizeof(array_v1), &array_v1, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, ver);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, 512 * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
	
	/*
	GLfloat vertices2[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3
		1,-1,-1,   1, 1,-1,  -1, 1,-1,  -1,-1,-1 }; // v4,v5,v6,v7
	/*GLubyte indices[] = { 0,1,2, 2,3,0,   // 36 of indices
		0,3,4, 4,5,0,
		0,5,6, 6,1,0,
		1,6,7, 7,2,1,
		7,4,3, 3,2,7,
		4,7,6, 6,5,4 };
		// activate and specify pointer to vertex array*/


	/*
	glVertex2d()
	GLuint ver;
	glGenBuffers(1, &ver);
	glBindBuffer(GL_ARRAY_BUFFER, ver);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), &vertices2, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, ver);
	glVertexPointer(3, GL_FLOAT, 0, vertices2);
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2));
	glDisableClientState(GL_VERTEX_ARRAY);
	
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices2);

	// draw a cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/*GLuint my_indices;
	glGenBuffers(1, &my_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 8, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint m_indices;
	glGenBuffers(1, &m_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);*/
	// deactivate vertex arrays after drawing
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/*
	glBegin(GL_TRIANGLES);

	glVertex3f(0.f, 1.f, 0.f); //A
	glVertex3f(0.f, 0.f, 0.f); //B
	glVertex3f(1.f, 1.f, 0.f); //C
	glVertex3f(1.f, 0.f, 0.f); //D
	glVertex3f(1.f, 0.f, -1.f); //E
	glVertex3f(1.f, 1.f, -1.f); //F
	glVertex3f(0.f, 1.f, -1.f); //G
	glVertex3f(0.f, 0.f, -1.f); //H
	*/
    /*glBegin(GL_TRIANGLES);
	glVertex3f(0.f, 1.f, 0.f); //A
	glVertex3f(0.f, 0.f, 0.f); //B
	glVertex3f(1.f, 1.f, 0.f); //C

	glVertex3f(0.f, 0.f, 0.f); //B
	glVertex3f(1.f, 0.f, 0.f); //D
	glVertex3f(1.f, 1.f, 0.f); //C

	glVertex3f(1.f, 1.f, 0.f); //C
	glVertex3f(1.f, 0.f, 0.f); //D
	glVertex3f(1.f, 0.f, -1.f); //E

	glVertex3f(1.f, 0.f, -1.f); //E
	glVertex3f(1.f, 1.f, -1.f); //F
	glVertex3f(1.f, 1.f, 0.f); //C

	glVertex3f(1.f, 1.f, 0.f); //C
	glVertex3f(1.f, 1.f, -1.f); //F
	glVertex3f(0.f, 1.f, -1.f); //G

	glVertex3f(1.f, 1.f, 0.f); //C
	glVertex3f(0.f, 1.f, -1.f); //G
	glVertex3f(0.f, 1.f, 0.f); //A

	glVertex3f(0.f, 1.f, -1.f); //G
	glVertex3f(0.f, 0.f, 0.f); //B
	glVertex3f(0.f, 1.f, 0.f); //A

	glVertex3f(0.f, 1.f, -1.f); //G
	glVertex3f(0.f, 0.f, -1.f); //H
	glVertex3f(0.f, 0.f, 0.f); //B

	glVertex3f(0.f, 0.f, -1.f); //H
	glVertex3f(1.f, 0.f, 0.f); //D
	glVertex3f(0.f, 0.f, 0.f); //B

	glVertex3f(0.f, 0.f, -1.f); //H
	glVertex3f(1.f, 0.f, -1.f); //E
	glVertex3f(1.f, 0.f, 0.f); //D

	glVertex3f(1.f, 1.f, -1.f); //F
	glVertex3f(1.f, 0.f, -1.f); //E
	glVertex3f(0.f, 1.f, -1.f); //G

	glVertex3f(1.f, 0.f, -1.f); //E
	glVertex3f(0.f, 0.f, -1.f); //H
	glVertex3f(0.f, 1.f, -1.f); //G

	glEnd();*/
	/*
	glBegin(GL_TRIANGLES);

	//glVertex3f(0.f, 1.f, 0.f); //A
	//glVertex3f(0.f, 0.f, 0.f); //B
	//glVertex3f(1.f, 1.f, 0.f); //C
	//glVertex3f(1.f, 0.f, 0.f); //D
	//glVertex3f(1.f, 0.f, -1.f); //E
	//glVertex3f(1.f, 1.f, -1.f); //F
	//glVertex3f(0.f, 1.f, -1.f); //G
	//glVertex3f(0.f, 0.f, -1.f); //H
	//glVertex3f(0.5f, 0.5f, -0.5f); //I

	glVertex3f(2.f, 0.f, 0.f); //B
	glVertex3f(2.f, 0.f, -1.f); //H
	glVertex3f(3.f, 0.f, 0.f); //D

	glVertex3f(3.f, 0.f, 0.f); //D
	glVertex3f(2.f, 0.f, -1.f); //H
	glVertex3f(3.f, 0.f, -1.f); //E

	glVertex3f(2.5f, 0.5f, -0.5f); //I
	glVertex3f(3.f, 0.f, 0.f); //D
	glVertex3f(3.f, 0.f, -1.f); //E

	glVertex3f(2.5f, 0.5f, -0.5f); //I
	glVertex3f(2.f, 0.f, 0.f); //B
	glVertex3f(3.f, 0.f, 0.f); //D

	glVertex3f(2.5f, 0.5f, -0.5f); //I
	glVertex3f(2.f, 0.f, -1.f); //H
	glVertex3f(2.f, 0.f, 0.f); //B

	glVertex3f(2.5f, 0.5f, -0.5f); //I
	glVertex3f(3.f, 0.f, -1.f); //E
	glVertex3f(2.f, 0.f, -1.f); //H




	glEnd();*/

	/*GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-10.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  5.0f, 0.0f,
	};


	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);*/

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
