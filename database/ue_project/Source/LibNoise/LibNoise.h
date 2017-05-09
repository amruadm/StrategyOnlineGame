#pragma once
#include "../../ThirdParty/LibNoise/Includes/noise.h" // переместить всё это дело в полноценные модули
#include "Runtime/Core/Public/Modules/ModuleManager.h"

class FLibNoiseGameModule : public IModuleInterface
{
public:

	//noise::module::Perlin* PerlinGenerator;

	virtual void StartupModule() override
	{
		//PerlinGenerator = new noise::module::Perlin();
	}

	virtual void ShutdownModule() override
	{
		//delete PerlinGenerator;
	}
};

//IMPLEMENT_PRIMARY_GAME_MODULE(FLibNoiseGameModule, LibNoise, "LibNoise");
//IMPLEMENT_GAME_MODULE(FLibNoiseGameModule, LibNoise);