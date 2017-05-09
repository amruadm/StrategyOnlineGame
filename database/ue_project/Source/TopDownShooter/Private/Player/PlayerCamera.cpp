// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Player/PlayerCamera.h"
#include "TopDownShooterCharacter.h"
#include "TopDownShooterPlayerController.h"

// Sets default values
APlayerCamera::APlayerCamera()
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("MainCamera");
	CameraComponent->AttachTo(RootComponent);
	CameraComponent->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));
	
	StrategyModeHeight = 800.0f;
}

// Called when the game starts or when spawned
void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCamera::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
		FHitResult hitResult;
		GetWorld()->LineTraceSingleByChannel(
			hitResult,		//result
			CameraComponent->GetComponentLocation(),	//start
			CameraComponent->GetComponentRotation().Vector() * 100000, //end
			ECollisionChannel::ECC_WorldStatic, //collision channel
			FCollisionQueryParams()
			);

		if (hitResult.GetActor() != nullptr)
		{
			SetActorLocation(hitResult.Location - CameraComponent->GetComponentRotation().Vector() * StrategyModeHeight);
		}
}

// Called to bind functionality to input
void APlayerCamera::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void APlayerCamera::MoveUp(float rate)
{

}

void APlayerCamera::MoveLeft(float rate)
{

}


UCameraComponent* APlayerCamera::GetCameraComponent() const
{
	return CameraComponent;
}
