// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project0 : ModuleRules
{
	public Project0(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "UMG", "Slate", "SlateCore", "Chaos", "GeometryCollectionEngine", "FieldSystemEngine", "LevelSequence", "MovieScene" });
	}
}
