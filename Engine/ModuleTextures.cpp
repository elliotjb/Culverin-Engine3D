#include "ModuleTextures.h"
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIl.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

ModuleTextures::ModuleTextures(bool start_enabled)
{
}

ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Start()
{
	ilInit();
	iluInit();
	ilutInit();

	ilutRenderer(ILUT_OPENGL);

	return true;
}

bool ModuleTextures::CleanUp()
{
	return true;
}

bool ModuleTextures::LoadTexture(const char * filename)
{
	return true;
}
