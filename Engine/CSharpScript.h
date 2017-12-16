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

class CSharpScript;

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
	ScriptVariable(const char* name, VarType type, VarAccess access, CSharpScript* script);
	virtual ~ScriptVariable();

	void SetMonoValue(void* newVal);

	void SetMonoField(MonoClassField* mfield);
	void SetMonoType(MonoType* mtype);

public:
	const char* name = nullptr;
	VarType type = Var_UNKNOWN;
	void* value = nullptr;
	VarAccess access = VarAccess::Var_PRIVATE;

private:
	//Mono properties to link with he script
	MonoClassField * monoField = nullptr;
	MonoType* monoType = nullptr;

	//To access the script
	CSharpScript* script = nullptr;
};


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

	//GET functions ------------------------------------
	MonoObject* GetMonoObject() const;

	// Functions set initial info ------------------------
	void SetDomain(MonoDomain* domain);
	void SetAssembly(MonoAssembly* assembly);
	void SetImage(MonoImage* image);
	void SetClass(MonoClass* klass);
	void SetClassName(std::string _name);
	void SetNameSpace(std::string _name_space);


	//Variables METHODS -------------------------------------------------
	void ResetScriptVariables();
	void GetScriptVariables();

	VarType GetTypeFromMono(MonoType* mtype);
	bool GetValueFromMono(ScriptVariable* variable, MonoClassField* mfield, MonoType* mtype);
	bool LinkVarToMono(ScriptVariable* variable, MonoClassField* mfield, MonoType* mtype);
	void SetVarValue(ScriptVariable* variable, void* new_val);
	// ------------------------------------------------------------------

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


