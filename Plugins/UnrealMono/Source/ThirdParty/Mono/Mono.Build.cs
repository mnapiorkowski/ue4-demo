// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class Mono : ModuleRules
{
	public string LibPath(string PlatformStr)
    {
		return Path.Combine(PluginDirectory, "Runtime", PlatformStr, "lib");
	}

	public string IncludePath
    {
		get { return Path.Combine(PluginDirectory, "Source", "ThirdParty", "Mono", "include"); }
	}

	public Mono(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.Add(Path.Combine(IncludePath, "mono-2.0"));

		string LibName, PlatformStr;
		LibName = PlatformStr = string.Empty;
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			LibName = "mono-2.0-sgen.lib";
			PlatformStr = "Win64";
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
		}

		if (!string.IsNullOrEmpty(LibName) && !string.IsNullOrEmpty(PlatformStr))
		{
			PublicAdditionalLibraries.Add(Path.Combine(LibPath(PlatformStr), LibName));
			RuntimeDependencies.Add(Path.Combine(LibPath(PlatformStr), LibName));
		}
	}
}
