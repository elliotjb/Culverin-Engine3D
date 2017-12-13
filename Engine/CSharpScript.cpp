#include "CSharpScript.h"
#include "Application.h"
#include "ModuleFS.h"
#include "ModuleImporter.h"


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
		CSObject = mono_object_new(App->importer->GetDomain(), CSClass);
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

void CSharpScript::SetClass(MonoClass* class_)
{
	CSClass = class_;
}

void CSharpScript::SetClassName(std::string name_)
{
	name = name_;
}

void CSharpScript::SetNameSpace(std::string name_space_)
{
	name_space = name_space_;
}

