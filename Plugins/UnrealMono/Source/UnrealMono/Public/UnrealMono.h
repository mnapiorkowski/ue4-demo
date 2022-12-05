#pragma once

#include "HAL/Platform.h"
#include "Modules/ModuleManager.h"

class UNREALMONO_API FUnrealMonoModule : public IModuleInterface
{
public:
	static bool MonoJitInit();
};