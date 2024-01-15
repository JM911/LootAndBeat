// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LootAndBeat : ModuleRules
{
	public LootAndBeat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
