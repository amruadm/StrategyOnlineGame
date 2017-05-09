// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TopDownShooter.h"
#include "Runtime/Engine/Classes/Engine/ActorChannel.h"
#include "TopDownShooterCharacter.h"
#include "Public/Weapons/Weapon.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Public/HUD/GameHUD.h"
#include "UnrealNetwork.h"

ATopDownShooterCharacter::ATopDownShooterCharacter()
{

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bReplicates = true;
	bReplicateMovement = true;

	InventorySize = 10;
}

void ATopDownShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	/*if (!IsLocallyControlled())
	{
		CursorToWorld->SetHiddenInGame(true);		
	}*/
	if (HasAuthority())
	{
		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(MainWeaponClass);
	}
}

void ATopDownShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentWeapon)
	{
		if (!CurrentWeapon->Owner)
		{
			AttachWeapon(CurrentWeapon);
		}
	}

	if (WaitForProcessAttack)
	{
		FVector dir = InputTarget - GetActorLocation();
		dir.Z = 0.0f;
		//FaceRotation(dir.Rotation(), 250.0f*DeltaSeconds);		
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), dir.Rotation(), DeltaSeconds, 20.0f));
	}
}

void ATopDownShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATopDownShooterCharacter, CurrentWeapon);
	DOREPLIFETIME(ATopDownShooterCharacter, InventorySize);
	DOREPLIFETIME(ATopDownShooterCharacter, Inventory);
}

bool ATopDownShooterCharacter::ReplicateSubobjects(UActorChannel * Channel, FOutBunch * Bunch, FReplicationFlags * RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UItem* item : Inventory)
	{
		if (item != nullptr)
		{
			WroteSomething |= Channel->ReplicateSubobject((UObject*)item, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void ATopDownShooterCharacter::MoveToDirection(FVector dir)
{
	if(CanInputMoving())
		AddMovementInput(dir * GetCharacterMovement()->GetMaxSpeed());
}

bool ATopDownShooterCharacter::CanInputMoving() const
{
	return !WaitForProcessAttack && Health > 0.0f;
}

AWeapon* ATopDownShooterCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

void ATopDownShooterCharacter::AttachWeapon(AWeapon* weapon)
{
	weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketName);
	weapon->Owner = this;
}

bool ATopDownShooterCharacter::Attack_Validate(FVector AttackPoint)
{
	return true;
}

void ATopDownShooterCharacter::Attack_Implementation(FVector AttackPoint)
{
	if (CurrentWeapon)
	{
		InputTarget = AttackPoint;
		WaitForProcessAttack = true;
		ClientSimulateAttack(AttackPoint);
	}
}

void ATopDownShooterCharacter::ClientSimulateAttack_Implementation(FVector AttackPoint)
{
	if (!IsLocallyControlled() && !HasAuthority())
	{
		if (CurrentWeapon)
		{
			InputTarget = AttackPoint;
			WaitForProcessAttack = true;
		}
	}
}

void ATopDownShooterCharacter::CreateInventory()
{
	if (HasAuthority())
	{
		Inventory.Init(nullptr, InventorySize);
	}
}