// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Buildings/BuildingHouse.h"
#include "Public/Units/CitizenUnit.h"

ABuildingHouse::ABuildingHouse()
{
	SpawnPoint = CreateDefaultSubobject<UVectorFieldComponent>(TEXT("SpawnPoint"));

	SpawnTimeInterval = 10.0f;
	MaxCitizens = 3;
}

void ABuildingHouse::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuildingHouse, Citizens);
	DOREPLIFETIME(ABuildingHouse, MaxCitizens);
}

void ABuildingHouse::BeginPlay()
{
	Super::BeginPlay();	
}

void ABuildingHouse::SpawnCitizen()
{
	if (Citizens.Num() < MaxCitizens && HasCompleted())
	{
		ACitizenUnit* newUnit = GetWorld()->SpawnActor<ACitizenUnit>(CitizenClass);
		if (newUnit)
		{
			newUnit->SetActorLocation(GetActorLocation() + SpawnPoint->GetComponentLocation());
			newUnit->SetTeamNum(GetTeamNum());
			newUnit->SetOwner(GetOwner());
			newUnit->OwnHouse = this;
			Citizens.Add(newUnit);
		}
	}
}

void ABuildingHouse::OnBuildingComplete_Implementation()
{
	Super::OnBuildingComplete_Implementation();
	if (HasAuthority())
	{
		FTimerDelegate SpawnDelegate = FTimerDelegate::CreateLambda([=]()
		{
			this->SpawnCitizen();
		});
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, SpawnDelegate, SpawnTimeInterval, true);
	}
}