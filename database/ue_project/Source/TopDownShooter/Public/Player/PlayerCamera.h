// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "PlayerCamera.generated.h"

UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API APlayerCamera : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCamera();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void MoveUp(float rate);

	void MoveLeft(float rate);

	UCameraComponent* GetCameraComponent() const;

	UPROPERTY(EditDefaultsOnly, Category="Game Camera")
	float StrategyModeHeight;

protected:

	UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	
};
