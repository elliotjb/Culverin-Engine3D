#include "CSharpScript.h"
#include "Application.h"
#include "ModuleFS.h"
#include "ModuleImporter.h"
#include "ImportScript.h"
#include "CompTransform.h"
#include "GameObject.h"
#include "Scene.h"

//SCRIPT VARIABLE UTILITY METHODS ------
ScriptVariable::ScriptVariable(const char* name, VarType type, VarAccess access, CSharpScript* script) : name(name), type(type), access(access), script(script)
{
}

ScriptVariable::~ScriptVariable()
{
}

void ScriptVariable::SetMonoValue(void* newVal)
{
	if (newVal != nullptr)
	{
		mono_field_set_value(script->GetMonoObject(), monoField, newVal);
	}

	else
	{
		LOG("[error] new value to set was nullptr");
	}

}

void ScriptVariable::SetMonoField(MonoClassField* mfield)
{
	if (mfield != nullptr)
	{
		monoField = mfield;
	}
	else
	{
		LOG("[error] MonoClassField* pointer was nullptr");
	}
}

void ScriptVariable::SetMonoType(MonoType* mtype)
{
	if (mtype != nullptr)
	{
		monoType = mtype;
	}
	else
	{
		LOG("[error] MonoType* pointer was nullptr");
	}
}

//CSHARP SCRIPT FUNCTIONS ---------------
CSharpScript::CSharpScript()
{
}


CSharpScript::~CSharpScript()
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

		//Create main Functions
		Start = CreateMainFunction("Start", DefaultParam, FunctionBase::CS_Start);
		Update = CreateMainFunction("Update", DefaultParam, FunctionBase::CS_Update);
		OnGUI = CreateMainFunction("OnGUI", DefaultParam, FunctionBase::CS_OnGUI);
		OnEnable = CreateMainFunction("OnEnable", DefaultParam, FunctionBase::CS_OnEnable);
		OnDisable = CreateMainFunction("OnDisable", DefaultParam, FunctionBase::CS_OnDisable);

		//Get Script Variables info (from c# to c++)
		GetScriptVariables();
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
			UpdateScriptVariables();
		}
		break;
	}
	case FunctionBase::CS_Update:
	{
		if (Update.method != nullptr)
		{
			DoFunction(Update.method, nullptr);
			UpdateScriptVariables();
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

bool CSharpScript::MonoObjectIsValid(MonoObject* object)
{
	if (object != nullptr)
	{
		//currentGameObject = gameObjects[object];
		return true;
	}
	return false;
}

MonoObject * CSharpScript::GetMonoObject() const
{
	return CSObject;
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

				//Set script info and functionality
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

//Release memory allocated from old variables
void CSharpScript::ResetScriptVariables()
{
	for (uint i = 0; i < variables.size(); i++)
	{
		RELEASE(variables[i]);
	}

	variables.clear();
	field_type.clear();
}

void CSharpScript::SetAttachedGameObject(GameObject * gameobject)
{
	attached_gameobject = gameobject;
	CreateSelfGameObject();
}

void CSharpScript::CreateSelfGameObject()
{
	MonoClass* c = mono_class_from_name(App->importer->iScript->GetCulverinImage(), "TheEngine", "TheGameObject");
	if (c)
	{
		MonoObject* new_object = mono_object_new(CSdomain, c);
		if (new_object)
		{
			CSSelfObject = new_object;
			gameObjects[CSSelfObject] = attached_gameobject;
		}
	}
}

void CSharpScript::GetScriptVariables()
{
	//Reset previour info
	ResetScriptVariables();

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

		//Insert this info pair into the map
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
		ScriptVariable* new_var = new ScriptVariable(mono_field_get_name(it->first), type, access, this);

		//Set its value
		GetValueFromMono(new_var, it->first, it->second);

		//Link to Mono properties
		LinkVarToMono(new_var, it->first, it->second);

		//Put it in variables vector
		variables.push_back(new_var);
	}
}

void CSharpScript::UpdateScriptVariables()
{
	uint count = 0;
	// From the map, update the value of each script
	for (std::map<MonoClassField*, MonoType*>::iterator it = field_type.begin(); it != field_type.end(); ++it)
	{
		//Set its value
		UpdateValueFromMono(variables[count++], it->first, it->second);
	}
}


VarType CSharpScript::GetTypeFromMono(MonoType* mtype)
{
	if (mtype != nullptr)
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

		if (name == "CulverinEditor.GameObject")
		{
			return VarType::Var_GAMEOBJECT;
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
	else
	{
		LOG("MonoType* mtype was nullptr");
		return VarType::Var_UNKNOWN;
	}
}

bool CSharpScript::GetValueFromMono(ScriptVariable* variable, MonoClassField* mfield, MonoType* mtype)
{
	if (variable != nullptr && mfield != nullptr && mtype != nullptr)
	{
		//Free memory
		if (variable->value != nullptr)
		{
			RELEASE(variable->value);
			variable->value = nullptr;
		}

		if (variable->type != VarType::Var_STRING && variable->type != VarType::Var_GAMEOBJECT)
		{
			//Allocate memory
			variable->value = new char[mono_type_stack_size(mtype, NULL)];

			//Set value of the variable by passing it as a reference in this function
			mono_field_get_value(CSObject, mfield, variable->value);
		}
		else if (variable->type == VarType::Var_STRING)
		{
			MonoString* str = nullptr;
			//Set value of the variable by passing it as a reference in this function
			mono_field_get_value(CSObject, mfield, &str);

			//Copy string into str_value (specific for strings)
			variable->str_value = mono_string_to_utf8(str);
		}
		else if (variable->type == VarType::Var_GAMEOBJECT)
		{
			variable->gameObject = nullptr;
			//Set value of the variable by passing it as a reference in this function
			//mono_field_get_value(CSObject, mfield, variable->gameObject);
			
		}
		return true;
	}
	else
	{
		LOG("[error] There is some null pointer.");
		return false;
	}
}

bool CSharpScript::UpdateValueFromMono(ScriptVariable * variable, MonoClassField * mfield, MonoType * mtype)
{
	if (variable != nullptr && mfield != nullptr && mtype != nullptr)
	{
		if (variable->type != VarType::Var_STRING && variable->type != VarType::Var_GAMEOBJECT)
		{
			//Set value of the variable by passing it as a reference in this function
			mono_field_get_value(CSObject, mfield, variable->value);
		}
		else if (variable->type == VarType::Var_GAMEOBJECT)
		{
			if (variable->gameObject != nullptr)
			{
				//Set value of the variable by passing it as a reference in this function
				mono_field_get_value(CSObject, mfield, variable->gameObject);
			}
		}
		else
		{
			MonoString* str = nullptr;

			//Set value of the variable by passing it as a reference in this function
			mono_field_get_value(CSObject, mfield, &str);

			//Copy string into str_value (specific for strings)
			variable->str_value = mono_string_to_utf8(str);
		}
		return true;
	}
	else
	{
		LOG("[error] There is some null pointer.");
		return false;
	}
}

bool CSharpScript::LinkVarToMono(ScriptVariable* variable, MonoClassField * mfield, MonoType * mtype)
{
	if (variable != nullptr && mfield != nullptr && mtype != nullptr)
	{
		variable->SetMonoField(mfield);
		variable->SetMonoType(mtype);

		return true;
	}
	else
	{
		LOG("[error] There is some null pointer.");
		return false;
	}
}

void CSharpScript::SetVarValue(ScriptVariable * variable, void * new_val)
{

}

void CSharpScript::SetCurrentGameObject(GameObject* current)
{
	currentGameObject = current;
}

void CSharpScript::CreateGameObject(MonoObject* object)
{
	GameObject* gameobject = App->scene->CreateGameObject();
	gameObjects[object] = gameobject;
}

MonoString* CSharpScript::GetName(MonoObject* object)
{
	if (!MonoObjectIsValid(object))
	{
		return nullptr;
	}
	if (currentGameObject == nullptr)
	{
		return nullptr;
	}
	return mono_string_new(CSdomain, currentGameObject->GetName());
}

MonoObject* CSharpScript::GetComponent(MonoObject* object, MonoReflectionType* type)
{
	if (!MonoObjectIsValid(object))
	{
		return nullptr;
	}

	if (currentGameObject == nullptr)
	{
		return nullptr;
	}

	MonoType* t = mono_reflection_type_get_type(type);
	std::string name = mono_type_get_name(t);

	const char* comp_name = "";

	if (name == "CulverinEditor.Transform")
	{
		comp_name = "Transform";
	}

	MonoClass* c = mono_class_from_name(App->importer->iScript->GetCulverinImage(), "CulverinEditor", comp_name);
	if (c)
	{
		MonoObject* new_object = mono_object_new(CSdomain, c);
		if (new_object)
		{
			return new_object;
		}
	}
	return nullptr;
}

MonoObject* CSharpScript::GetPosition(MonoObject* object)
{
	MonoClass* c = mono_class_from_name(App->importer->iScript->GetCulverinImage(), "CulverinEditor", "Vector3");
	if (c)
	{
		MonoObject* new_object = mono_object_new(App->importer->iScript->GetDomain(), c);
		if (new_object)
		{
			MonoClassField* x_field = mono_class_get_field_from_name(c, "x");
			MonoClassField* y_field = mono_class_get_field_from_name(c, "y");
			MonoClassField* z_field = mono_class_get_field_from_name(c, "z");

			CompTransform* transform = (CompTransform*)currentGameObject->GetComponentTransform();
			float3 new_pos;
			new_pos = transform->GetPos();

			if (x_field) mono_field_set_value(new_object, x_field, &new_pos.x);
			if (y_field) mono_field_set_value(new_object, y_field, &new_pos.y);
			if (z_field) mono_field_set_value(new_object, z_field, &new_pos.z);

			return new_object;
		}
	}
	return nullptr;
}

// We need to pass the MonoObject* to get a reference on act
void CSharpScript::SetPosition(MonoObject* object, MonoObject* vector3)
{
	MonoClass* c = mono_object_get_class(vector3);
	MonoClassField* x_field = mono_class_get_field_from_name(c, "x");
	MonoClassField* y_field = mono_class_get_field_from_name(c, "y");
	MonoClassField* z_field = mono_class_get_field_from_name(c, "z");

	float3 new_pos;

	if (x_field) mono_field_get_value(vector3, x_field, &new_pos.x);
	if (y_field) mono_field_get_value(vector3, y_field, &new_pos.y);
	if (z_field) mono_field_get_value(vector3, z_field, &new_pos.z);

	CompTransform* transform = (CompTransform*)currentGameObject->GetComponentTransform();
	transform->SetPos(new_pos);
}