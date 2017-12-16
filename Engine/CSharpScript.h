#ifndef _CSHARPSCRIPT_
#define _CSHARPSCRIPT_

// Info-> http://docs.go-mono.com/?link=root:/embed

#define DefaultParam 0

#include <string>
#include <vector>
#include <map>
#include <mono/metadata/metadata.h>
#include <mono/metadata/object.h>
#include <mono/metadata/attrdefs.h>

enum FunctionBase
{
	CS_Start, CS_Update, CS_OnGUI, CS_OnEnable, CS_OnDisable
};

enum VarType
{
	Var_UNKNOWN = -1,
	Var_INT = 0,
	Var_FLOAT = 1,
	Var_BOOL = 2,
	Var_STRING = 3,
	Var_CLASS = 4
};

enum VarAccess
{
	Var_PUBLIC = 0,
	Var_PRIVATE,
};

class ScriptVariable
{
public:
	ScriptVariable(const char* name, VarType type, VarAccess access);
	virtual ~ScriptVariable();

	template<class TYPE>
	TYPE GetValue() const;

	template<class TYPE>
	void SetValue(TYPE new_value);

public:
	const char* name = nullptr;
	VarType type = Var_UNKNOWN;
	void* value = nullptr;
	VarAccess access = VarAccess::Var_PRIVATE;
};

template<class TYPE>
inline TYPE ScriptVariable::GetValue() const
{
	if (type == VarType::Var_INT)
	{
		return (int)value;
	}
	else if (type == VarType::Var_FLOAT)
	{
		return (float)value;
	}
	else if (type == VarType::Var_BOOL)
	{
		return (bool)value;
	}
	else if (type == VarType::Var_STRING)
	{
		return (const char*)value;
	}
	else
	{
		LOG("Unknown variable type of '%s'", name.c_str());
		return NULL;
	}
}

template<class TYPE>
inline void ScriptVariable::SetValue(TYPE new_value)
{
	if (type == VarType::Var_INT)
	{
		value = (int)new_value;
	}
	else if (type == VarType::Var_FLOAT)
	{
		value = (float)new_value;
	}
	else if (type == VarType::Var_BOOL)
	{
		value = (bool)new_value;
	}
	else if (type == VarType::Var_STRING)
	{
		value = (const char*)new_value;
	}
	else
	{
		LOG("Unknown variable type of '%s'", name.c_str());
	}
}

struct MainMonoMethod
{
	FunctionBase type;
	MonoMethod* method = nullptr;
};

class CSharpScript
{
public:
	CSharpScript();
	~CSharpScript();

	//Importing/Reimporting/Initializing --------
	void LoadScript();
	bool ReImport(std::string pathdll);

	MainMonoMethod CreateMainFunction(std::string function, int parameters, FunctionBase type);
	void DoMainFunction(FunctionBase function);
	void DoFunction(MonoMethod* function, void ** parameter);

	// Functions set initial info ------------------------
	void SetDomain(MonoDomain* domain);
	void SetAssembly(MonoAssembly* assembly);
	void SetImage(MonoImage* image);
	void SetClass(MonoClass* klass);
	void SetClassName(std::string _name);
	void SetNameSpace(std::string _name_space);

	// Variable setting-getting functions ----------------
	template<class TYPE>
	void SetValue(const char* variable_name, TYPE value)
	{
		// find the  field in the Entity class
		MonoClassField* field = mono_class_get_field_from_name(CSClass, variable_name);
		if (field)
		{
			// set the field's value
			mono_field_set_value(CSObject, field, &value);
		}
	}

	template<class TYPE>
	TYPE GetValue(const char* variable_name)
	{
		// find the  field in the Entity class
		MonoClassField* field = mono_class_get_field_from_name(CSClass, variable_name);
		if (field)
		{
			TYPE value;

			// set the field's value
			mono_field_get_value(CSObject, field, &value);

			return value;
		}

		return NULL;
	}

	//Pass from csharp to c++ variables
	void ResetScriptVariables();
	void GetScriptVariables();

	VarType GetTypeFromMono(MonoType* mtype);
	void GetValueFromMono(ScriptVariable* variable, MonoClassField* mfield, MonoType* mtype);

public:
	//Variables/Info containers (public to have access through other modules)
	std::vector<ScriptVariable*> variables;

private:
	//Auxiliar map to fill variables vector with info
	std::map<MonoClassField*, MonoType*> field_type;
	std::string name;
	std::string name_space;

	MonoDomain* CSdomain = nullptr;
	MonoAssembly* CSassembly = nullptr;
	MonoImage* CSimage = nullptr;
	MonoClass* CSClass = nullptr;
	MonoObject* CSObject = nullptr;

	// Main Functions
	MainMonoMethod Start;
	MainMonoMethod Update;
	MainMonoMethod OnGUI;
	MainMonoMethod OnEnable;
	MainMonoMethod OnDisable;
};

#endif


