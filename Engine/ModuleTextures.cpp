#include "Globals.h"
#include "ModuleTextures.h"
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIl.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

ModuleTextures::ModuleTextures(bool start_enabled)
{
	name = "Textures";
}

ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Start()
{
	bool ret = true;

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("DevIL version is different\n");
		ret = false;
	}

	ilInit();
	iluInit();
	ilutInit();

	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool ModuleTextures::CleanUp()
{
	return true;
}

GLuint ModuleTextures::LoadTexture(const char * filename)
{
	ILuint imageID;
	GLuint textureID;
	ILenum error;

	ilGenImages(1, &imageID); 	
	ilBindImage(imageID); 

	if (ilLoadImage(filename))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		//Flip the image into the right way
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		if (!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE))
		{
			error = ilGetError();
			LOG("Image conversion failed - IL reportes error: %i, %s", error, iluErrorString(error));
			exit(-1);
		}

		// Texture Generation
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
	}

	else
	{
		error = ilGetError();
		LOG("Image Load failed - IL reportes error: %i, %s", error, iluErrorString(error));
	}

	//RELEASE MEMORY used by the image
	ilDeleteImages(1, &imageID); 

	LOG("Texture CREATION SUCCESSFUL");

	return true;
}
