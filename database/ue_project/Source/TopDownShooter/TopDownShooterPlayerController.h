// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "TopDownShooterPlayerController.generated.h"

UCLASS(Blueprintable)
class ATopDownShooterPlayerController : public APlayerController, public ITeamObjectInterface
{
	GENERATED_BODY()

public:
	ATopDownShooterPlayerController();

	class ATopDownShooterCharacter* GetHeroPawn();

	class AGameHUD* GetGameHUD() const;

	class AGamePlayerState* GetPlayerState() const;

	virtual int GetTeamNum() const override { return TeamNum; }

	virtual void SetTeamNum(int number) override { TeamNum = number; }

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void BeginSampleBuilding(TSubclassOf<class ABuilding> BuildingClass);

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void EndSampleBuilding();

	UFUNCTION(Server, Reliable, WithValidation)
	void ChangeMode();

	UFUNCTION(Server, Reliable, WithValidation)
	void MakeBuilding(FVector Location, FRotator Rotation, TSubclassOf<class ABuilding> BuildingClass);
	
	virtual void Possess(APawn* aPawn) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=RTSGame)
	bool IsStrategyMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
	TArray<TSubclassOf<class ABuilding>> GameBuildings;
	
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector StartLMouseMoveLocation;
protected:

	UPROPERTY(Replicated)
	int TeamNum;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	UPROPERTY(ReplicatedUsing= OnRep_CamMode)
	uint32 StrategyCameraMode;
	ATopDownShooterCharacter* HeroCharacter;
	class ACitizenUnit* SelectedUnit;
	class ITeamObjectInterface* LastOverlaped;

	bool IsSamplingBuild;
	bool IsRMouseDown;

	UFUNCTION(Client, Reliable)
	void ClientChangeMode();

	virtual void BeginPlay() override;
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void OnAttackPressed();
	void OnAttackReleased();

	void OnSetToggleModePressed();

	void OnMoveUp(float rate);
	void OnMoveRight(float rate);

	void OnOpenInventoryPressed();
	void OnOpenInventoryReleased();

	void OnLMouseDownPressed();
	void OnLMouseDownReleased();

	void OnRMouseDownPressed();
	void OnRMouseDownReleased();

	UFUNCTION()
	void OnRep_CamMode();	
};


