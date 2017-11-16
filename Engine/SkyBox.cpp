#include "SkyBox.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRenderer3D.h"

SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}


void SkyBox::InitSkybox()
{
	skybox[SkyBoxSide::SKY_LEFT] = App->textures->LoadSkyboxTexture("Images/Skybox/left.png");
	skybox[SkyBoxSide::SKY_BACK] = App->textures->LoadSkyboxTexture("Images/Skybox/back.png");
	skybox[SkyBoxSide::SKY_RIGHT] = App->textures->LoadSkyboxTexture("Images/Skybox/right.png");
	skybox[SkyBoxSide::SKY_FRONT] = App->textures->LoadSkyboxTexture("Images/Skybox/front.png");
	skybox[SkyBoxSide::SKY_TOP] = App->textures->LoadSkyboxTexture("Images/Skybox/top.png");
	skybox[SkyBoxSide::SKY_BOTTOM] = App->textures->LoadSkyboxTexture("Images/Skybox/bottom.png");
}

void SkyBox::DrawSkybox(float size, float3 pos)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//back face GOOD
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_BACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 0); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 1); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glEnd();

	//left face GOOD
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_LEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 0); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(0, 0); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glEnd();

	//front face GOOD
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
	glTexCoord2f(0, 1); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(1, 0); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 0); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glEnd();

	//top face GOOD
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_TOP]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 0); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 1); glVertex3f(-size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(size / 2 + pos.x, size / 2 + pos.y, -size / 2 + pos.z);
	glEnd();

	//bottom face GOOD
	glBindTexture(GL_TEXTURE_2D, skybox[SkyBoxSide::SKY_BOTTOM]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);
	glTexCoord2f(0, 1); glVertex3f(size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 1); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, -size / 2 + pos.z);
	glTexCoord2f(1, 0); glVertex3f(-size / 2 + pos.x, -size / 2 + pos.y, size / 2 + pos.z);

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


void SkyBox::DeleteSkyboxTex()
{
	glDeleteTextures(6, &skybox[0]);
}
