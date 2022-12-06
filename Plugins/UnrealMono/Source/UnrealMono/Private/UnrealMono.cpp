#include "UnrealMono.h"
#include "Modules/ModuleManager.h"

MonoDomain* FUnrealMonoModule::MonoJitInit(FString file)
{
	return mono_jit_init(TCHAR_TO_ANSI(*file));
}

MonoAssembly* FUnrealMonoModule::MonoDomainAssemblyOpen(MonoDomain* domain, FString name)
{
	return mono_domain_assembly_open(domain, TCHAR_TO_ANSI(*name));
}

MonoImage* FUnrealMonoModule::MonoAssemblyGetImage(MonoAssembly* assembly)
{
	return mono_assembly_get_image(assembly);
}

MonoMethodDesc* FUnrealMonoModule::MonoMethodDescNew(FString name, bool include_namespace)
{
	return mono_method_desc_new(TCHAR_TO_ANSI(*name), include_namespace);
}

MonoMethod* FUnrealMonoModule::MonoMethodDescSearchInImage(MonoMethodDesc* method_desc, MonoImage* image)
{
	return mono_method_desc_search_in_image(method_desc, image);
}

MonoObject* FUnrealMonoModule::MonoRuntimeInvoke(MonoMethod* method, void* obj, void** params, MonoObject** exc)
{
	return mono_runtime_invoke(method, obj, params, exc);
}

void* FUnrealMonoModule::MonoObjectUnbox(MonoObject* object)
{
	return mono_object_unbox(object);
}

void FUnrealMonoModule::MonoJitCleanup(MonoDomain* domain)
{
	mono_jit_cleanup(domain);
}

IMPLEMENT_MODULE(FUnrealMonoModule, UnrealMono)