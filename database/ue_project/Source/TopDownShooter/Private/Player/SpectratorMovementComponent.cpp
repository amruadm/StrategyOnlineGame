// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Player/SpectratorMovementComponent.h"
#include "TopDownShooterPlayerController.h"
#include "Public/Player/SpectratorCameraPawn.h"
#include "Public/Player/SpectratorCameraComponent.h"

USpectratorMovementComponent::USpectratorMovementComponent()
{
	MaxSpeed = 1000.f;
	Acceleration = 6000.f;
	Deceleration = 6000.f;

}

void USpectratorMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	ATopDownShooterPlayerController* PlayerController = Cast<ATopDownShooterPlayerController>(PawnOwner->GetController());
	if (PlayerController && PlayerController->IsLocalController())
	{
		if (!bInitialLocationSet)
		{
			PawnOwner->SetActorRotation(PlayerController->GetControlRotation());
			PawnOwner->SetActorLocation(PlayerController->GetSpawnLocation());
			bInitialLocationSet = true;
		}

		FVector MyLocation = UpdatedComponent->GetComponentLocation();
		ASpectratorCameraPawn* SpectatorPawn = Cast<ASpectratorCameraPawn>(PlayerController->GetSpectatorPawn());
		if ((SpectatorPawn != NULL) && (SpectatorPawn->GetStrategyCameraComponent() != NULL))
		{
			SpectatorPawn->GetStrategyCameraComponent()->ClampCameraLocation(PlayerController, MyLocation);
		}
		UpdatedComponent->SetWorldLocation(MyLocation, false);
	}
}