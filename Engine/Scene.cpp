#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "imgui.h"
#include "Primitive.h"
#include "Gl3W\include\glew.h"
#include "imgui_impl_sdl_gl3.h"

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
	Plane_p p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	//glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);

	//glVertex3f(0.f, 1.f, 0.f); //A
	//glVertex3f(0.f, 0.f, 0.f); //B
	//glVertex3f(1.f, 1.f, 0.f); //C
	//glVertex3f(1.f, 0.f, 0.f); //D
	//glVertex3f(1.f, 0.f, -1.f); //E
	//glVertex3f(1.f, 1.f, -1.f); //F
	//glVertex3f(0.f, 1.f, -1.f); //G
	//glVertex3f(0.f, 0.f, -1.f); //H

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

	glEnd();


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




	glEnd();

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



	return UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	return true;
}
