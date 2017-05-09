// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Weapons/MeeleWeapon.h"
#include "TopDownShooterCharacter.h"




void AMeeleWeapon::Attack(FVector TargetPoint)
{
	if (!Owner) return;
	
	FCollisionQueryParams params;
	params.AddIgnoredActor(Owner);
	FHitResult hitResult;

	FVector dir = TargetPoint - Owner->GetActorLocation();
	dir.Normalize();

	GetWorld()->LineTraceSingleByChannel(
		hitResult,		//result
		Owner->GetActorLocation(),	//start
		Owner->GetActorLocation() + dir * Distance, //end
		ECollisionChannel::ECC_Pawn, //collision channel
		params
		);

	if (hitResult.GetActor() != nullptr)
	{		
		ATopDownShooterCharacter* target = Cast<ATopDownShooterCharacter>(hitResult.GetActor());
		if (target)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString("Damage it!!!"));
			target->TakeDamage(Damage,
							FDamageEvent(),
							Owner->Controller, 
							Owner);
			if(damageParticle)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), damageParticle, target->GetActorLocation());
		}
	}
}

void AMeeleWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}