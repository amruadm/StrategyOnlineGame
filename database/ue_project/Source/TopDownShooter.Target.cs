// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TopDownShooterTarget : TargetRules
{
    /*public List<string> PublicDependencyModuleNames = new List<string>();
    public List<string> PrivateDependencyModuleNames = new List<string>();
    public List<string> PrivateIncludePathModuleNames = new List<string>();
    public int MinFilesUsingPrecompiledHeaderOverride = 1;
    public bool bFasterWithoutUnity = true;*/
    public TopDownShooterTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
        /*PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI", "RenderCore", "ShaderCore", "CustomMeshComponent" });
        PrivateDependencyModuleNames.AddRange(new string[] { "CustomMeshComponent" });
        PrivateIncludePathModuleNames.AddRange(new string[] { "CustomMeshComponent" });        */
    }

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
        OutExtraModuleNames.Add("TopDownShooter");
	}
}
