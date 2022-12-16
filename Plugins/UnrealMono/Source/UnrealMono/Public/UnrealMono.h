#pragma once

#include "HAL/Platform.h"
#include "Modules/ModuleManager.h"

THIRD_PARTY_INCLUDES_START
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/mono-config.h"
#include "mono/metadata/debug-helpers.h"
THIRD_PARTY_INCLUDES_END

namespace UnrealMono
{
	class UNREALMONO_API Module : public IModuleInterface
	{
	private:
		MonoDomain* Domain;

	protected:

		virtual void StartupModule() override;
		virtual void ShutdownModule() override;

	public:
		MonoImage* LoadAssembly(FString Path);
		MonoMethod* FindMethod(MonoImage* Image, FString MethodDescStr, bool IncludeNamespace);
		void* InvokeMethod(MonoMethod* Method, void* Object, void** Params, MonoObject** Exception);
	};

}
