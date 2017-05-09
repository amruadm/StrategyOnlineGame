// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "SpectratorCameraPawn.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ASpectratorCameraPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
	ASpectratorCameraPawn();

	/** event call on move forward input */
	virtual void MoveForward(float Val) override;

	/** event call on strafe right input */
	virtual void MoveRight(float Val) override;

	/** Add custom key bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpectratorCameraComponent* StrategyCameraComponent;

public:

	virtual void Tick(float DeltaTime) override;

	/** Handles the mouse scrolling down. */
	void OnMouseScrollUp();

	/** Handles the mouse scrolling up. */
	void OnMouseScrollDown();

	/* Returns a pointer to the strategy camera component the pawn has. */
	class USpectratorCameraComponent* GetStrategyCameraComponent();
	
	
};
