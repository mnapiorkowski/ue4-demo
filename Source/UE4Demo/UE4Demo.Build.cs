// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class UE4Demo : ModuleRules
{
	private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath( Path.Combine( ModulePath, "../../ThirdParty/" ) ); }
    }

	public UE4Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		LoadMono(Target);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}

	public bool LoadMono(ReadOnlyTargetRules Target)
    {
		Console.WriteLine(ThirdPartyPath);
		string LibrariesPath = Path.Combine(ThirdPartyPath, "Mono", "lib");
		PublicSystemLibraryPaths.Add(LibrariesPath);

		Console.WriteLine(Path.Combine(LibrariesPath, "mono-2.0-sgen.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "mono-2.0-sgen.lib"));
		//PublicDelayLoadDLLs.Add(Path.Combine(LibrariesPath, "mono-2.0-sgen.dll"));
		//PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "mono-2.0-boehm.lib"));
		//PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "libmono-static-boehm.lib"));
		//PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "libmono-static-sgen.lib"));
		//PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "MonoPosixHelper.lib"));

		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Mono", "include", "mono-2.0"));

        return true;
    }
}
