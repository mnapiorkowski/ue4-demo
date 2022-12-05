// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class MonoRuntime : ModuleRules
{
	public string LibPath
    {
		get { return Path.Combine(ModuleDirectory, "lib"); }
	}

	public string IncludePath
    {
		get { return Path.Combine(ModuleDirectory, "include"); }
	}

	public MonoRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.Add(Path.Combine(IncludePath, "mono-2.0"));

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, "mono-2.0-sgen.lib"));
			RuntimeDependencies.Add(Path.Combine(LibPath, "mono-2.0-sgen.lib"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
		}
	}
}
