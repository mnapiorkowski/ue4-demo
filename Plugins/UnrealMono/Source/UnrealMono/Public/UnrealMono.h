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

		MonoImage* Image;

		void* InvokeGetResult(void* Object, MonoMethod* Method, void** Params);
		
	protected:
		virtual void StartupModule() override;

		virtual void ShutdownModule() override;

	public:
		void LoadAssembly(FString Path);

		MonoMethod* FindMethod(FString MethodDescStr);

		template <typename T, typename... ArgsT> 
		T* InvokeMethod(void* Object, MonoMethod* Method, ArgsT*... Args);
		
		template <typename T, typename... ArgsT> 
		T* InvokeStaticMethod(MonoMethod* Method, ArgsT*... Args);

		template <typename T, typename... ArgsT> 
		T* FindInvokeMethod(void* Object, FString MethodDescStr, ArgsT*... Args);

		template <typename T, typename... ArgsT> 
		T* FindInvokeStaticMethod(FString MethodDescStr, ArgsT*... Args);
	};

	template <typename T, typename... ArgsT>
	T* Module::InvokeMethod(void* Object, MonoMethod* Method, ArgsT*... Args)
	{
		TArray<void*> Params = { (void*)Args... };
		return (T*)Module::InvokeGetResult(Object, Method, Params.GetData());
	}

	template <typename T, typename... ArgsT>
	T* Module::InvokeStaticMethod(MonoMethod* Method, ArgsT*... Args)
	{
		return Module::InvokeMethod<T>(nullptr, Method, Args...);
	}

	template <typename T, typename... ArgsT>
	T* Module::FindInvokeMethod(void* Object, FString MethodDescStr, ArgsT*... Args)
	{
		MonoMethod* Method = Module::FindMethod(MethodDescStr);
		return Module::InvokeMethod<T>(Object, Method, Args...);
	}

	template <typename T, typename... ArgsT>
	T* Module::FindInvokeStaticMethod(FString MethodDescStr, ArgsT*... Args)
	{
		MonoMethod* Method = Module::FindMethod(MethodDescStr);
		return Module::InvokeMethod<T>(nullptr, Method, Args...);
	}
}
