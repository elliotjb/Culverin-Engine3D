#include "CSharpScript.h"
#include "Application.h"
#include "ModuleFS.h"
#include "ModuleImporter.h"
#include "ImportScript.h"

//SCRIPT VARIABLE UTILITY METHODS -----
ScriptVariable::ScriptVariable()
{
}

ScriptVariable::~ScriptVariable()
{
}



//CSHARP SCRIPT FUNCTIONS ---------------
CSharpScript::CSharpScript()
{
}


CSharpScript::~CSharpScript()
{
}

void CSharpScript::InitCSharp(std::string CSharpFile)
{

}


void CSharpScript::LoadScript()
{
	if (CSClass)
	{
		CSObject = mono_object_new(App->importer->iScript->GetDomain(), CSClass);
	}
	if (CSObject)
	{
		mono_runtime_object_init(CSObject);
		// Create main Functions
		Start = CreateMainFunction("Start", DefaultParam, FunctionBase::CS_Start);
		Update = CreateMainFunction("Update", DefaultParam, FunctionBase::CS_Update);
		OnGUI = CreateMainFunction("OnGUI", DefaultParam, FunctionBase::CS_OnGUI);
		OnEnable = CreateMainFunction("OnEnable", DefaultParam, FunctionBase::CS_OnEnable);
		OnDisable = CreateMainFunction("OnDisable", DefaultParam, FunctionBase::CS_OnDisable);
	}
}

MainMonoMethod CSharpScript::CreateMainFunction(std::string function, int parameters, FunctionBase type)
{
	MainMonoMethod Newmethod;
	if (CSClass != nullptr)
	{
		Newmethod.method = mono_class_get_method_from_name(CSClass, function.c_str(), parameters);
		Newmethod.type = type;
	}
	return Newmethod;
}

void CSharpScript::DoMainFunction(FunctionBase function)
{
	switch (function)
	{
	case FunctionBase::CS_Start:
	{
		if (Start.method != nullptr)
		{
			DoFunction(Start.method, nullptr);
		}
		break;
	}
	case FunctionBase::CS_Update:
	{
		//GetScriptVariables();

		if (Update.method != nullptr)
		{
			DoFunction(Update.method, nullptr);
		}
		break;
	}
	case FunctionBase::CS_OnGUI:
	{
		if (OnGUI.method != nullptr)
		{
			DoFunction(OnGUI.method, nullptr);
		}
		break;
	}
	case FunctionBase::CS_OnEnable:
	{
		if (OnEnable.method != nullptr)
		{
			DoFunction(OnEnable.method, nullptr);
		}
		break;
	}
	case FunctionBase::CS_OnDisable:
	{
		if (OnDisable.method != nullptr)
		{
			DoFunction(OnDisable.method, nullptr);
		}
		break;
	}
	}
}

void CSharpScript::DoFunction(MonoMethod* function, void ** parameter)
{
	MonoObject* exception = nullptr;
	// Do Main Function
	mono_runtime_invoke(function, CSObject, parameter, &exception);
	if (exception)
	{
		mono_print_unhandled_exception(exception);
	}
}

void CSharpScript::SetDomain(MonoDomain* domain)
{
	CSdomain = domain;
}

void CSharpScript::SetAssembly(MonoAssembly* assembly)
{
	CSassembly = assembly;
}

void CSharpScript::SetImage(MonoImage* image)
{
	CSimage = image;
}

void CSharpScript::SetClass(MonoClass* klass)
{
	CSClass = klass;
}

void CSharpScript::SetClassName(std::string _name)
{
	name = _name;
}

void CSharpScript::SetNameSpace(std::string _name_space)
{
	name_space = _name_space;
}

void CSharpScript::GetScriptVariables()
{

	void* iter = nullptr;
	int num_fields = mono_class_num_fields(CSClass);
	int num_methods = mono_class_num_methods(CSClass);
	int num_properties = mono_class_num_properties(CSClass);

	const char* class_name = mono_class_get_name(CSClass);
	int count = 0;
	do
	{
		public_variables.push_back(mono_class_get_fields(CSClass, &iter));
		count++;
	} while (count < num_fields);

	for (uint i = 0; i < public_variables.size(); i++)
	{
		/*int value = -1;
		mono_field_get_value(CSObject, public_variables[i], &value);
		pv_name_type.insert(std::pair<const char*, ScriptVariable>(mono_field_get_name(public_variables[i]), (void*)value));*/
		//pv_name.push_back(mono_field_get_name(public_variables[i]));
		//pv_type.push_back(mono_field_get_type(public_variables[i]));
	}
}

