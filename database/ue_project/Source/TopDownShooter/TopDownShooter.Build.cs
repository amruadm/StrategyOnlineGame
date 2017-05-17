// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class TopDownShooter : ModuleRules
{
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
    }

    public TopDownShooter(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent", "RHI", "RenderCore", "ShaderCore", "AIModule" });        

        //PublicDependencyModuleNames.Add("LibNoise");

        if (UEBuildConfiguration.bWithServerCode == true)
        {
            //AddLib(Target, "Lua", "lua5.1.lib", "lua5.1.lib");
            //AddLib(Target, "Postgres", "libpq.lib", "libpq.lib");
        }        
    }

    private void AddLib(TargetInfo Target, string libFolderName, string libName86, string libName64)
    {
        string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
        string LibrariesPath = Path.Combine(ThirdPartyPath, libFolderName, "Libraries", PlatformString);
        string libFile = (Target.Platform == UnrealTargetPlatform.Win64) ? libName64 : libName86;

        PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, libFile));
    }
}
