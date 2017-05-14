// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/GameUnit.h"
#include "Animation/AnimInstance.h"


// Sets default values
AGameUnit::AGameUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	isProcessing = false;

	Strength = 1;
	Agility = 1;
	Intellect = 1;
	Exp = 0;
}

// Called when the game starts or when spawned
void AGameUnit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGameUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameUnit, TeamNum);
	DOREPLIFETIME(AGameUnit, Health);
	DOREPLIFETIME(AGameUnit, isProcessing);
	DOREPLIFETIME(AGameUnit, Place);
	DOREPLIFETIME(AGameUnit, Target);
}

float AGameUnit::GetMaxSpeed() const
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		return Movement->GetMaxSpeed();
	}
	return 0.0f;
}

float AGameUnit::GetMaxHealth() const
{
	return GetClass()->GetDefaultObject<AGameUnit>()->Health;
}

float AGameUnit::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	float res = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (HasDied()) return 0.0f;
	if (HasAuthority())
	{
		// Main damage process
		Health -= DamageAmount;
		if (Health <= 0.0f)
		{
			Health = 0.0f;
			Die(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		}
	}
	else
	{
		// Damage simulation on client
	}

	return res;
}

void AGameUnit::Die(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	Destroy();
}

bool AGameUnit::HasDied() const
{
	return Health <= 0.0f;
}

void AGameUnit::BeginProcessAction(EGameUnitAction ActionType, AActor* TargetActor)
{	
	if (Role == ROLE_Authority)
	{
		Target = TargetActor;
		MulticastBeginProcessAction(ActionType);
	}
}

void AGameUnit::MulticastBeginProcessAction_Implementation(EGameUnitAction ActionType)
{
	isProcessing = true;
	OnBeginProcessAction.Broadcast(ActionType);
}

void AGameUnit::ProcessAction(EGameUnitAction ActionType)
{
	isProcessing = false;
	OnProcessAction.Broadcast(ActionType);
	if (ActionType == EGameUnitAction::UA_Attack)
	{
		ProcessAttack();
	}		
}

void AGameUnit::EndAction(EGameUnitAction ActionType)
{
	OnEndAction.Broadcast(ActionType);
}

void AGameUnit::PlaceTo(AActor* NewPlace)
{
	Place = NewPlace;
	if (Place)
	{
		SetActorHiddenInGame(true);
	}
}

void AGameUnit::ExitFromPlace()
{
	IUnitPlaceInterface* place = InterfaceCast<IUnitPlaceInterface>(Place);
	if (place) 
		place->OnUnplace(this);
	Place = nullptr;
	SetActorHiddenInGame(false);
}

bool AGameUnit::IsPlaced() const
{
	return Place != nullptr;
}

void AGameUnit::PlayAnimation(EGameUnitAction AnimActionType, int AnimIndex)
{
	if (Anims.Contains(AnimActionType))
	{
		if (Anims[AnimActionType].Num() > 0)
		{
			MulticastPlayAnimation(AnimActionType, AnimIndex);
		}
	}
}

void AGameUnit::PlayRandomAnimation(EGameUnitAction AnimActionType)
{
	if (Anims.Contains(AnimActionType))
	{
		int r = FMath::RandRange(0, Anims[AnimActionType].Num() - 1);
		PlayAnimation(AnimActionType, r);
	}
}

void AGameUnit::MulticastPlayAnimation_Implementation(EGameUnitAction AnimActionType, int AnimIndex)
{
	GetMesh()->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage(Anims[AnimActionType][AnimIndex], NAME_Default);
}

void AGameUnit::ProcessAttack()
{
	
}

void AGameUnit::Selected_Implementation()
{
	if (USkeletalMeshComponent* Mesh = GetMesh())
	{
		Mesh->SetRenderCustomDepth(true);
		if (ITeamObjectInterface* TeamObject = Cast<ITeamObjectInterface>(GetWorld()->GetFirstPlayerController()))
		{
			if (TeamObject->GetTeamNum() == TeamNum)
			{
				Mesh->SetCustomDepthStencilValue(STENCIL_FRIENDLY);
				return;
			}
		}
		Mesh->SetCustomDepthStencilValue(STENCIL_ENEMY);
	}
}

void AGameUnit::Unselected_Implementation()
{
	if (USkeletalMeshComponent* Mesh = GetMesh())
	{
		Mesh->SetRenderCustomDepth(false);		
		
	}
}

FControlData AGameUnit::GetUIData_Implementation()
{
	return FControlData();
}