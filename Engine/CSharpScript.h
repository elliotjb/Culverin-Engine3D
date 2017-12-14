#ifndef _CSHARPSCRIPT_
#define _CSHARPSCRIPT_

// Info-> http://docs.go-mono.com/?link=root:/embed

#define DefaultParam 0

#include <string>
#include <vector>
#include <map>
#include <mono/metadata/metadata.h>
#include <mono/metadata/object.h>

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
};

struct VarValue
{
	int iVal = 0;
	float fVal = 0.0f;
	bool bVal = false;
	const char* strVal = nullptr;
};

class ScriptVariable
{
public:
	ScriptVariable(const char* name, VarType type, VarValue val);
	virtual ~ScriptVariable();

	template<class TYPE>
	TYPE GetValue() const;

	template<class TYPE>
	void SetValue(TYPE new_value);

public:
	const char* name = nullptr;
	VarType type = Var_UNKNOWN;

private:
	VarValue value;
};

template<class TYPE>
inline TYPE ScriptVariable::GetValue() const
{
	if (type == VarType::Var_INT)
	{
		return (int)value.iVal;
	}
	else if (type == VarType::Var_FLOAT)
	{
		return (float)value.fVal;
	}
	else if (type == VarType::Var_BOOL)
	{
		return (bool)value.bVal;
	}
	else if (type == VarType::Var_STRING)
	{
		return (const char*)value.strVal;
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
		value.iVal = (int)new_value;
	}
	else if (type == VarType::Var_FLOAT)
	{
		value.fVal = (float)new_value;
	}
	else if (type == VarType::Var_BOOL)
	{
		value.bVal = (bool)new_value;
	}
	else if (type == VarType::Var_STRING)
	{
		value.strVal = (const char*)new_value;
	}
	else
	{
		LOG("Unknown variable type of '%s'",name.c_str());
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

	void InitCSharp(std::string CSharpFile);
	void LoadScript();

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
	void GetScriptVariables();

	VarType GetTypeFromMono(MonoType* mtype);
	VarValue GetValueFromMono(MonoClassField* mfield);


private:
	std::string name;
	std::string name_space;

	MonoDomain* CSdomain = nullptr;
	MonoAssembly* CSassembly = nullptr;
	MonoImage* CSimage = nullptr;
	MonoClass* CSClass = nullptr;
	MonoObject* CSObject = nullptr;

	std::vector<ScriptVariable*> variables;
	
	std::map<MonoClassField*, MonoType*> field_type;
	std::vector<const char*> pv_name;
	std::vector<MonoType*> pv_type;
	
	// Main Functions
	MainMonoMethod Start;
	MainMonoMethod Update;
	MainMonoMethod OnGUI;
	MainMonoMethod OnEnable;
	MainMonoMethod OnDisable;
};

#endif

