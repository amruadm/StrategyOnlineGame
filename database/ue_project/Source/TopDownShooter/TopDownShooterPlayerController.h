// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "TopDownShooterPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectDelegate, const TArray<TScriptInterface<class ISelectable>> &, Selected);

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

	void SelectUnits(TArray<AActor*> Units);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerController")
	TArray<AActor*> GetSelectedUnits() const
	{
		return SelectedUnits;
	}

	void UnselectAll();

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

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FSelectDelegate OnSelected;
protected:

	UParticleSystem* TargetPointParticles;

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

	TArray<AActor*> SelectedUnits;

	UFUNCTION(Client, Reliable)
	void ClientChangeMode();

	virtual void BeginPlay() override;
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface


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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCommandTarget(const TArray<AActor*> & CommandUnits, struct FCommandTarget Target);
	virtual void ServerCommandTarget_Implementation(const TArray<AActor*> & CommandUnits, struct FCommandTarget Target);
	virtual bool ServerCommandTarget_Validate(const TArray<AActor*> & CommandUnits, struct FCommandTarget Target);
};


