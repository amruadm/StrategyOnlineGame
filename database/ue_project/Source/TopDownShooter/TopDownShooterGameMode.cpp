// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TopDownShooter.h"
#include "TopDownShooterGameMode.h"
#include "TopDownShooterPlayerController.h"
#include "TopDownShooterCharacter.h"
#include "Public/Player/PlayerCamera.h"
#include "Public/Player/GamePlayerState.h"
#include "Public/Player/SpectratorCameraPawn.h"
#include "Public/Units/CitizenUnit.h"
#include "Public/Buildings/Building.h"
#include "Public/HUD/GameHUD.h"
#include "Runtime/Engine/Public/EngineUtils.h"

/*
#ifdef WITH_SERVER_CODE

#include "../../ThirdParty/Lua/Includes/lua.hpp"
#include "../../ThirdParty/Lua/Includes/LuaBridge/LuaBridge.h"
#include "../../ThirdParty/Postgres/Includes/libpq-fe.h"

using namespace luabridge;

#endif
*/

ATopDownShooterGameMode::ATopDownShooterGameMode()
{	
	HUDClass = AGameHUD::StaticClass();
	PlayerControllerClass = ATopDownShooterPlayerController::StaticClass();
	SpectatorClass = ASpectratorCameraPawn::StaticClass();
	DefaultPawnClass = ASpectratorCameraPawn::StaticClass();
	PlayerStateClass = AGamePlayerState::StaticClass();
	lastTeamIndex = 0;
}

void ATopDownShooterGameMode::BeginPlay()
{
	Super::BeginPlay();	

#ifdef WITH_SERVER_CODE

	//-----------------DATABASE BEGIN

	/*PGconn* conn = PQconnectdb("dbname=postgres host=localhost user=postgres password=qwerty");
	//PGresult* res;
	if (PQstatus(conn) == CONNECTION_BAD)
	{		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "DB BAD CONNECTION");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "CONNECTED TO DB");
	}

	//-----------------DB END

	//----------------LUA RUNNING
	FString luaRoot = FPaths::Combine(*(FPaths::GameDir()), *FString("Scripts"));
	FString initScript = FPaths::Combine(*luaRoot, *FString("Init.lua"));

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, TCHAR_TO_ANSI(*initScript));
	LuaRef lref = getGlobal(L, "test_str");
	std::string test_str = lref.cast<std::string>();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(UTF8_TO_TCHAR(test_str.c_str())));*/
	//------------------LUA END	

#endif
}

void ATopDownShooterGameMode::PostLogin(APlayerController * newPlayer)
{
	Super::PostLogin(newPlayer);
	
	FActorSpawnParameters params;
	params.Owner = newPlayer;

	ATopDownShooterPlayerController* pl = Cast<ATopDownShooterPlayerController>(newPlayer);
	if (pl)
	{
		pl->SetTeamNum(lastTeamIndex);

		APlayerStart* playerStart = nullptr;
		for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			playerStart = Cast<APlayerStart>(*ActorItr);
			break;
		}
		if (playerStart && CitizenClass)
		{
			for (int i = 0; i < 1; i++)
			{
				ACitizenUnit* citizen = GetWorld()->SpawnActor<ACitizenUnit>(CitizenClass);
				if (citizen)
				{
					citizen->SetTeamNum(lastTeamIndex);
					citizen->SetOwner(pl);
					citizen->SetActorLocation(playerStart->GetActorLocation());
				}
			}
		}
	}

	/*ATopDownShooterCharacter* character = GetWorld()->SpawnActor<ATopDownShooterCharacter>(HeroClass, params);
	if (character != nullptr)
	{		
		APlayerStart* playerStart = nullptr;
		for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			playerStart = Cast<APlayerStart>(*ActorItr);
			break;
		}
		character->SetActorLocation(playerStart->GetActorLocation());		
		character->SetTeamNum(lastTeamIndex);
		character->CreateInventory();
		
	}	*/

	lastTeamIndex++;
}

void ATopDownShooterGameMode::Tick(float DeltaSeconds)
{
	for (TActorIterator<ABuilding> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABuilding* b = *ActorItr;
		break;
	}
}