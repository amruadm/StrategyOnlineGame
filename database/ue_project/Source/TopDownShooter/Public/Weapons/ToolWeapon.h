// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/MeeleWeapon.h"
#include "ToolWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AToolWeapon : public AMeeleWeapon
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category="ToolWeaoin")
	class AWorkerUnit* GetOwnerWorker() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameResource")
	int GetEfficiency() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ToolWeapon")
	int Efficiency;
	
};
