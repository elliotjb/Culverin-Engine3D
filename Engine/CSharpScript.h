#ifndef _CSHARPSCRIPT_
#define _CSHARPSCRIPT_

// Info-> http://docs.go-mono.com/?link=root:/embed

#define DefaultParam 0

#include <string>

#include <mono/metadata/metadata.h>
#include <mono/metadata/object.h>

enum FunctionBase
{
	CS_Start, CS_Update, CS_OnGUI, CS_OnEnable, CS_OnDisable
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

	void InitCSharp(std::string CSharpFile);
	void LoadScript();


	MainMonoMethod CreateMainFunction(std::string function, int parameters, FunctionBase type);
	void DoMainFunction(FunctionBase function);
	void DoFunction(MonoMethod* function, void ** parameter);

	// Functions set initial info
	void SetDomain(MonoDomain* domain);
	void SetAssembly(MonoAssembly* assembly);
	void SetImage(MonoImage* image);
	void SetClass(MonoClass* class_);
	void SetClassName(std::string name_);
	void SetNameSpace(std::string name_space_);





private:

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
	MainMonoMethod	OnEnable;
	MainMonoMethod	OnDisable;
};

#endif