// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/HUD/GameHUD.h"
#include "TopDownShooterPlayerController.h"
#include "Public/Buildings/Building.h"

AGameHUD::AGameHUD()
{
	bSelection = false;
}

bool AGameHUD::OnToggleHeroInventory_Implementation()
{
	return true;
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!GEngine->GameViewport)
		return;
	
	PlayerController = Cast<ATopDownShooterPlayerController>(GetOwningPlayerController());
	if (!PlayerController) return;

	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

	FVector2D ScreenSize;
	GEngine->GameViewport->GetViewportSize(ScreenSize);

	if (PlayerController->WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		bSelection = true;
		SelectionStartPos = MousePosition;
	}

	if (PlayerController->WasInputKeyJustReleased(EKeys::LeftMouseButton))
	{
		bSelection = false;
		
		Select(SelectionStartPos, MousePosition);
	}

	if (bSelection)
	{
		FCanvasBoxItem SelectionBox(SelectionStartPos, MousePosition - SelectionStartPos);
		SelectionBox.SetColor(FLinearColor(0.0f, 1.0f, 0.0f, 0.5f));
		Canvas->DrawItem(SelectionBox);
	}
}

void AGameHUD::Select(FVector2D StartPos, FVector2D EndPos)
{
	TArray<AActor*> Actors;
	TArray<AActor*> OutSelectableActors;
	TArray<AActor*> PendingToSelectActors;

	if (FVector2D::Distance(StartPos, EndPos) < 10.0f)
	{
		FHitResult HitResult;
		if (PlayerController->GetHitResultUnderCursor(ECC_WorldDynamic, false, HitResult))
		{
			if (Cast<ISelectable>(HitResult.Actor.Get()))
			{
				PendingToSelectActors.Add(HitResult.Actor.Get());					
			}
			else return;
		}
		else return;
	}
	else
	{
		if (GetActorsInSelectionRectangle<AActor>(StartPos, EndPos, Actors, true))
		{
			for (auto Pawn : Actors)
			{
				ISelectable* Selectable = Cast<ISelectable>(Pawn);
				if (Selectable)
				{
					PendingToSelectActors.Add(Pawn);
				}
			}
		}
	}
	if (PendingToSelectActors.Num() > 1)
	{
		for (auto Pawn : PendingToSelectActors)
		{
			if (ISelectable* Selectable = Cast<ISelectable>(Pawn))
			{
				if (Selectable->Execute_CanBeMultipleSelected(Pawn))
					OutSelectableActors.Add(Pawn);
			}
		}
	}
	else
	{
		OutSelectableActors = PendingToSelectActors;
	}
	PlayerController->SelectUnits(OutSelectableActors);
}

void AGameHUD::DrawBuildingItem(FVector2D Position, float Size, TSubclassOf<class ABuilding> BuildingClass)
{
	if (ABuilding* BuildingObject = BuildingClass->GetDefaultObject<ABuilding>())
	{
		DrawTexture(BuildingObject->Icon, Position.X, Position.Y, Size, Size, 0.0f, 0.0f, 1.0f, 1.0f);
		if (IsMouseOverlapp(Position, FVector2D(Size, Size)) && HasMouseClicked())
		{
			PlayerController->BeginSampleBuilding(BuildingClass);
		}
	}
}

bool AGameHUD::IsMouseDown() const
{
	return PlayerController->IsInputKeyDown(EKeys::LeftMouseButton);
}

bool AGameHUD::HasMouseClicked() const
{
	return PlayerController->WasInputKeyJustPressed(EKeys::LeftMouseButton);
}