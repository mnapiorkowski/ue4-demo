#include "UnrealMono.h"
#include "Modules/ModuleManager.h"

void UnrealMono::Module::StartupModule()
{
	mono_config_parse(nullptr);
	Domain = mono_jit_init("TestDomain");
	if (!Domain)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnrealMono failed to initialize Mono runtime."));
	}
}

void UnrealMono::Module::ShutdownModule()
{
	mono_jit_cleanup(Domain);
}

void UnrealMono::Module::LoadAssembly(FString Path)
{
	MonoAssembly* Assembly = mono_domain_assembly_open(Domain, TCHAR_TO_ANSI(*Path));
	if (!Assembly)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnrealMono failed to open %s assembly."), *Path);
	}
	else
	{
		Image = mono_assembly_get_image(Assembly);
		if (!Image)
		{
			UE_LOG(LogTemp, Warning, TEXT("UnrealMono failed to get the image of assembly."));
		}
	}
}

MonoMethod* UnrealMono::Module::FindMethod(FString MethodDescStr)
{
	MonoMethod* Method = nullptr;

	int32 DotIdx = INDEX_NONE, ColonIdx = INDEX_NONE;
	MethodDescStr.FindChar(TCHAR(':'), ColonIdx);
	bool IncludeNamespace = MethodDescStr.FindChar(TCHAR('.'), DotIdx) && DotIdx < ColonIdx;

	MonoMethodDesc* MethodDesc = mono_method_desc_new(TCHAR_TO_ANSI(*MethodDescStr), IncludeNamespace);
	if (!MethodDesc)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnrealMono failed to get %s descriptor."), *MethodDescStr);
	}
	else
	{
		Method = mono_method_desc_search_in_image(MethodDesc, Image);
		if (!Method)
		{
			UE_LOG(LogTemp, Warning, TEXT("UnrealMono failed to find %s in assembly image."), *MethodDescStr);
		}
	}
	return Method;
}

void* UnrealMono::Module::InvokeGetResult(void* Object, MonoMethod* Method, void** Params)
{
	void* Result = nullptr;
	MonoObject* Exception = nullptr;

	MonoObject* ResultObject = mono_runtime_invoke(Method, Object, Params, &Exception);
	if (Exception)
	{
		UE_LOG(LogTemp, Warning, TEXT("An unhandled exception was thrown in managed code."));
	}
	else if (ResultObject)
	{
		Result = mono_object_unbox(ResultObject);
	}
	return Result;
}

IMPLEMENT_MODULE(UnrealMono::Module, UnrealMono)