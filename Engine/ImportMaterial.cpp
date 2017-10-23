#include "ImportMaterial.h"
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

bool ImportMaterial::Import(char* file)
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
			std::string name = file;
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

bool ImportMaterial::Import(const void * buffer, uint size, std::string & output_file)
{
	return false;
}

bool ImportMaterial::Load(const char* exported_file, Texture* resource)
{
	return false;
}

bool ImportMaterial::LoadCheckers(Texture* resource)
{
	return false;
}
