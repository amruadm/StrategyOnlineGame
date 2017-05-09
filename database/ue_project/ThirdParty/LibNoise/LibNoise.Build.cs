using System.IO;
using UnrealBuildTool;
 
public class LibNoise: ModuleRules
{
    private string ModulePath
    {
        get { return Path.GetDirectoryName( RulesCompiler.GetModuleFilename( this.GetType().Name ) ); }
    }
 
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath( Path.Combine( ModulePath, "../ThirdParty/" ) ); }
    }
 
    [Constructor]

	public bool LoadBobsMagic(TargetInfo Target)
    {
        bool isLibrarySupported = false;
 
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;
 
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "LibNoise", "Libraries", PlatformString);
            string libFile = (Target.Platform == UnrealTargetPlatform.Win64) ? "libnoise64.lib" : "libnoise.lib";

            //libnoise
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, libFile)); 
        }
 
        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add( Path.Combine( ThirdPartyPath, "LibNoise", "Includes" ) );
        }
 
        Definitions.Add(string.Format( "WITH_LIBNOISE_BINDING={0}", isLibrarySupported ? 1 : 0 ) );
 
        return isLibrarySupported;
    }

}