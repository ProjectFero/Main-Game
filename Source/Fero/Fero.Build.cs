// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Fero : ModuleRules
{
	public Fero(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",    
                "RHI", 
                 "RenderCore", 
                 "ShaderCore"
     });
	}
}
