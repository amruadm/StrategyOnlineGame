// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Units/GameUnit.h"
#include "CitizenUnit.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ACitizenUnit : public AGameUnit
{
	GENERATED_BODY()
	
	
public:

	/*
		@param TSubclassOf<ACitizenUnit> NewProfession class of prof character
		@return instance of new prof unit
	*/
	UFUNCTION(BlueprintCallable, Category = "CitizenUnit")
	ACitizenUnit* TakeProfession(TSubclassOf<ACitizenUnit> NewProfession);

	UPROPERTY(BlueprintReadOnly, Replicated, Category="CitizenUnit")
	class ABuildingHouse* OwnHouse;

	

};
