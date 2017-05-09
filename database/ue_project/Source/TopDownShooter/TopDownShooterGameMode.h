// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "TopDownShooterGameMode.generated.h"

UCLASS(minimalapi)
class ATopDownShooterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATopDownShooterGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	TSubclassOf<class ACitizenUnit> HeroClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	TSubclassOf<class ACitizenUnit> CitizenClass;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* newPlayer) override;

	virtual void Tick(float DeltaSeconds) override;

protected:

	int lastTeamIndex;
};



