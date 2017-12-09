#include "stdafx.h"
#include "Scripting.h"

#include <string>
#include <iostream>
#include <windows.h>
#include <direct.h>

// Mono Include
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/jit/jit.h>

#pragma comment(lib, "mono-2.0-sgen.lib")

SCRIPTING_MANAGER bool ScriptingManager::CompileFile(const char * path)
{
	// Get the path of the project ----
	char my_path[FILENAME_MAX];
	_getcwd(my_path, FILENAME_MAX);

	std::string scriptPath = my_path;
	scriptPath += path;

	// Compile the script -------------
	std::string command = my_path;
	command += "/Mono/bin/mcs " + scriptPath + " -target:library";
	if (system(command.c_str()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//// INIT MONO --------------------------------
//// Use the standard configuration
//mono_config_parse(NULL);
//
//std::string lib = my_path;
//lib += "/Mono/lib";
//std::string etc = my_path;
//etc += "/Mono/etc";
//
//// Setup the default directories for mono use here for now the directories of your Mono installation
//mono_set_dirs(lib.c_str(), etc.c_str());
//MonoDomain* domain = mono_jit_init_version("CoolTest", "v4.0.30319");
//
//// Open the Assembly with your scripts
//assembly = mono_domain_assembly_open(domain, "Example.dll");
//
//// Get the Image
//image = mono_assembly_get_image(assembly);
//
//// Find the Entity class in the image
//MonoClass* entityClass = mono_class_from_name(image, "Example", "Entity");
//
//// allocate memory for one Entity instance
//MonoObject* entityInstance = mono_object_new(domain, entityClass);
//
//// find the Entity class constructor method that takes one parameter
//MonoMethod* constructorMethod = mono_class_get_method_from_name(entityClass, ".ctor", 1);
//
//// create a MonoString that will be passed to the constructor as an argument
//MonoString* name = mono_string_new(mono_domain_get(), "Giorgos");
//void* args[1];
//args[0] = name;
//
//// finally, invoke the constructor
//MonoObject* exception = NULL;
//mono_runtime_invoke(constructorMethod, entityInstance, args, &exception);
//
//// find the Process method that takes zero parameters
//MonoMethod* processMethod = mono_class_get_method_from_name(entityClass, "Process", 0);
//exception = nullptr;
//
//// invoke the method
//// if invoking static methods, then the second argument must be NULL
//mono_runtime_invoke(processMethod, entityInstance, nullptr, &exception);
//
//// check for any thrown exception
//if (exception)
//{
//	std::cout << mono_string_to_utf8(mono_object_to_string(exception, nullptr)) << std::endl;
//}
//
//// find the GetName method
//MonoMethod* getNameMethod = mono_class_get_method_from_name(entityClass, "GetName", 0);
//exception = nullptr;
//MonoString* ret = (MonoString*)mono_runtime_invoke(getNameMethod, entityInstance, nullptr, &exception);
//char* c = mono_string_to_utf8(ret);
//std::cout << "Value of 'name' is " << c << std::endl;
//
//// free the memory allocated from mono_string_to_utf8 ()
//mono_free(c);
//
////// find the Id field in the Entity class
////MonoClassField* idField = mono_class_get_field_from_name(entityClass, "Id");
////int value = 42;
//
////// set the field's value
////mono_field_set_value(entityObject, idField, &value);
//
////int result;
////mono_field_get_value(entityObject, idField, &result);
////std::cout << "Value of 'Id' is " << result << std::endl;
//
//
//// shutdown mono
//mono_jit_cleanup(domain);


