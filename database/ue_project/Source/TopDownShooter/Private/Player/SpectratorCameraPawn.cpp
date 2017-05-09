// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Player/SpectratorCameraPawn.h"
#include "Public/Player/SpectratorMovementComponent.h"
#include "Public/Player/SpectratorCameraComponent.h"

ASpectratorCameraPawn::ASpectratorCameraPawn()
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	StrategyCameraComponent = CreateDefaultSubobject<USpectratorCameraComponent>(TEXT("StrategyCameraComponent"));
	StrategyCameraComponent->AttachTo(RootComponent);
	StrategyCameraComponent->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	PrimaryActorTick.bCanEverTick = true;
}

void ASpectratorCameraPawn::Tick(float DeltaTime)
{
	FHitResult hitResult;

	GetWorld()->LineTraceSingleByChannel(
		hitResult,		//result
		StrategyCameraComponent->GetComponentLocation(),	//start
		-StrategyCameraComponent->GetComponentRotation().Vector() * 100000, //end
		ECollisionChannel::ECC_WorldStatic, //collision channel
		FCollisionQueryParams()
		);

	//DrawDebugLine(GetWorld(), StrategyCameraComponent->GetComponentLocation(),
	//-StrategyCameraComponent->GetComponentRotation().Vector() * 100000, FColor::Green);

	/*if (hitResult.GetActor() != NULL)
	{
	SetActorLocation(hitResult.Location + StrategyCameraComponent->GetComponentRotation().Vector()*3000);
	}*/
}

void ASpectratorCameraPawn::OnMouseScrollUp()
{
	StrategyCameraComponent->OnZoomIn();
}

void ASpectratorCameraPawn::OnMouseScrollDown()
{
	StrategyCameraComponent->OnZoomOut();
}


void ASpectratorCameraPawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ASpectratorCameraPawn::OnMouseScrollUp);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ASpectratorCameraPawn::OnMouseScrollDown);

	InputComponent->BindAxis("MoveForward", this, &ASpectratorCameraPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASpectratorCameraPawn::MoveRight);
}


void ASpectratorCameraPawn::MoveForward(float Val)
{
	StrategyCameraComponent->MoveForward(Val);
}


void ASpectratorCameraPawn::MoveRight(float Val)
{
	StrategyCameraComponent->MoveRight(Val);
}

USpectratorCameraComponent* ASpectratorCameraPawn::GetStrategyCameraComponent()
{
	check(StrategyCameraComponent != NULL);
	return StrategyCameraComponent;
}

