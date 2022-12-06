#pragma once

#include "HAL/Platform.h"
#include "Modules/ModuleManager.h"

THIRD_PARTY_INCLUDES_START
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/mono-config.h"
#include "mono/metadata/debug-helpers.h"
THIRD_PARTY_INCLUDES_END

class UNREALMONO_API FUnrealMonoModule : public IModuleInterface
{
public:
	MonoDomain* MonoJitInit(FString file);
	MonoAssembly* MonoDomainAssemblyOpen(MonoDomain* domain, FString name);
	MonoImage* MonoAssemblyGetImage(MonoAssembly* assembly);
	MonoMethodDesc* MonoMethodDescNew(FString name, bool include_namespace);
	MonoMethod* MonoMethodDescSearchInImage(MonoMethodDesc* method_desc, MonoImage* image);
	MonoObject* MonoRuntimeInvoke(MonoMethod* method, void* obj, void** params, MonoObject** exc);
	void* MonoObjectUnbox(MonoObject* object);
	void MonoJitCleanup(MonoDomain* domain);
};
