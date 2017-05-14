// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"



/**
 * 
 */
UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AGameHUD();

	virtual void DrawHUD() override;

	UFUNCTION(BlueprintNativeEvent, Category="Input")
	bool OnToggleHeroInventory();

protected:

	FVector2D MousePosition;

	bool bSelection;
	FVector2D SelectionStartPos;
	class ATopDownShooterPlayerController* PlayerController;

	void Select(FVector2D StartPos, FVector2D EndPos);

	FORCEINLINE bool IsMouseOverlapp(FVector2D Position, FVector2D Size) const
	{
		return MousePosition.X >= Position.X && MousePosition.Y >= Position.Y
			&& MousePosition.X <= (Position.X + Size.X)
			&& MousePosition.Y <= (Position.Y + Size.Y);
	}

	FORCEINLINE bool IsMouseDown() const;
	FORCEINLINE bool HasMouseClicked() const;

	FORCEINLINE void DrawBuildingItem(FVector2D Position, float Size, TSubclassOf<class ABuilding> BuildingClass);
};
