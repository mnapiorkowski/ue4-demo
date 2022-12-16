// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealMono : ModuleRules
{
	public UnrealMono(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] { });
				
		
		PrivateIncludePaths.AddRange(new string[] { });
			
		
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core",
			"Mono",
		});
			
		
		PrivateDependencyModuleNames.AddRange(new string[] { });
		
		DynamicallyLoadedModuleNames.AddRange(new string[] { });
	}
}
