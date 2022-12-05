#include "UnrealMono.h"
#include "Modules/ModuleManager.h"

THIRD_PARTY_INCLUDES_START
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/mono-config.h"
#include "mono/metadata/debug-helpers.h"
THIRD_PARTY_INCLUDES_END

bool FUnrealMonoModule::MonoJitInit()
{
	MonoDomain* domain = mono_jit_init("TestDomain");
	if (!domain) 
		return false;
	return true;
}

IMPLEMENT_MODULE(FUnrealMonoModule, UnrealMono)