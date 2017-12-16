#include "CSharpScript.h"
#include "Application.h"
#include "ModuleFS.h"
#include "ModuleImporter.h"
#include "ImportScript.h"

//SCRIPT VARIABLE UTILITY METHODS ------
ScriptVariable::ScriptVariable(const char* name, VarType type, VarAccess access): name(name), type(type), access(access)
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
		GetScriptVariables();

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

bool CSharpScript::ReImport(std::string pathdll)
{
	MonoAssembly* assembly_ = mono_domain_assembly_open(App->importer->iScript->GetDomain(), pathdll.c_str());
	if (assembly_)
	{
		MonoImage* image_ = mono_assembly_get_image(assembly_);
		if (image_)
		{
			std::string classname_, name_space_;
			MonoClass* entity_ = App->importer->iScript->GetMonoClassFromImage(image_, name_space_, classname_);
			if (entity_)
			{
				SetImage(image_);
				SetClass(entity_);
				SetClassName(classname_);
				SetNameSpace(name_space_);
				LoadScript();
				SetAssembly(assembly_);
				SetDomain(App->importer->iScript->GetDomain());
			}
			else
			{
				LOG("[error]Failed loading class %s\n", classname_.c_str());
				return false;
			}
		}
		else
		{
			LOG("[error] Error with Image");
			return false;
		}
	}
	else
	{
		LOG("[error] Error with Assembly");
		return false;
	}
	return true;
}

void CSharpScript::GetScriptVariables()
{
	static uint32_t field_attr_public = 0x0006;
	static uint32_t flags;

	MonoClassField* field = nullptr;
	MonoType* type = nullptr;
	void* iter = nullptr;

	int num_fields = mono_class_num_fields(CSClass);
	
	//Fill field-type map from the script to after get its respective info
	for (uint i = 0; i < num_fields; i++)
	{
		field = mono_class_get_fields(CSClass, &iter);
		type = mono_field_get_type(field);

		field_type.insert(std::pair<MonoClassField*, MonoType*>(field, type));
	}

	// From the previous map, fill VariablesScript vector that will contain info (name, type, value) of each variable
	for (std::map<MonoClassField*, MonoType*>::iterator it = field_type.begin(); it != field_type.end(); ++it)
	{
		VarType type = GetTypeFromMono(it->second);
		VarAccess access = VarAccess::Var_PRIVATE;

		//Set info about accessibility of the variable -> DOESN'T WORK!!!
		flags = mono_field_get_flags(field);
		if ((flags & MONO_FIELD_ATTR_PUBLIC))
		{
			access = VarAccess::Var_PUBLIC;
		}
		else if ((flags & MONO_FIELD_ATTR_PRIVATE))
		{
			access = VarAccess::Var_PRIVATE;
		}

		//Create variable
		ScriptVariable* new_var = new ScriptVariable(mono_field_get_name(it->first), type, access);
	
		//Set its value
		GetValueFromMono(new_var, it->first, it->second);

		//Put it in variables vector
		variables.push_back(new_var);
	}
}

VarType CSharpScript::GetTypeFromMono(MonoType* mtype)
{
	std::string name = mono_type_get_name(mtype);
	if (name == "System.Int32")
	{
		return VarType::Var_INT;
	}
		
	if (name == "System.Single")
	{
		return VarType::Var_FLOAT;
	}

	if (name == "System.Boolean")
	{
		return VarType::Var_BOOL;
	}

	if (name == "System.String")
	{
		return VarType::Var_STRING;
	}
	else
	{
		LOG("Unknown variable type");
		return VarType::Var_UNKNOWN;
	}
}

void CSharpScript::GetValueFromMono(ScriptVariable* variable, MonoClassField* mfield, MonoType* mtype)
{
	//Free memory
	RELEASE(variable->value);
	variable->value = nullptr;

	//Allocate memory
	variable->value = new char[mono_type_stack_size(mtype, NULL)];

	//Set value of the script into its respective variable
	mono_field_get_value(CSObject, mfield, variable->value);
}

