#include "ImportMaterial.h"
#include "CompMaterial.h"
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIl.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

ImportMaterial::ImportMaterial()
{
}


ImportMaterial::~ImportMaterial()
{
}

//update_status MaterialImporter::Update(float dt)
//{
//
//
//
//	return UPDATE_CONTINUE;
//}

bool ImportMaterial::Import(const char* file, const char* name_file)
{
	bool ret = false;
	char* buffer;
	uint size_file = App->fs->LoadFile(file, &buffer);
	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size_file))
	{
		ILuint size;
		ILubyte *data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
		if (size > 0)
		{
			data = new ILubyte[size]; // allocate data buffer
			std::string name = name_file;
			name = App->fs->FixName_directory(name);
			name = App->fs->FixExtension(name, ".dds");
			if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
				ret = App->fs->SaveFile((char*)data, name, size, IMPORT_DIRECTORY_LIBRARY_MATERIALS);
			RELEASE_ARRAY(data);
		}
	}

	//return ret;

	return false;
}

bool ImportMaterial::Load(const char* file, CompMaterial* materialComp)
{
	Texture texture;
	ILuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	ILboolean success = ilLoadImage(file);

	//If the image is correctly loaded
	if (success)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		glBindTexture(GL_TEXTURE_2D, textureID);
		success = ilConvertImage(ilGetInteger(IL_IMAGE_FORMAT), IL_UNSIGNED_BYTE);

		if (!success)
		{
			ILenum error = ilGetError();
			LOG("Error, Loading texture: %s", file);
			LOG("Error, %s", iluErrorString(error));
			ilDeleteImages(1, &textureID);
			return false;
		}

		//Set the texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,	0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),	ilGetInteger(IL_IMAGE_HEIGHT), 
			0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		glGenerateMipmap(GL_TEXTURE_2D);

		LOG("Texture, %s loaded!", file);
	}
	else
	{
		ILenum error = ilGetError();
		LOG("Error, Loading texture: %s", file);
		LOG("Error, %s", iluErrorString(error));
		ilDeleteImages(1, &textureID);
		return false;
	}

	//Delete the image 
	ilDeleteImages(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Texture id and name, now no need path, because that the texture was imported.
	texture.id = textureID;
	texture.name = file;

	//Add the texture to the component
	if (textureID != 0)
	{
		materialComp->AddTexture(texture);
	}

	return true;
}

bool ImportMaterial::LoadCheckers(Texture* resource)
{
	return false;
}
