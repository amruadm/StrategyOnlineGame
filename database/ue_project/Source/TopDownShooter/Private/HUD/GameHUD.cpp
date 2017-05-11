// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/HUD/GameHUD.h"
#include "TopDownShooterPlayerController.h"

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

	FVector2D MousePosition;
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
	TArray<APawn*> Pawns;
	TArray<APawn*> OutSelectablePawns;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Select");
	if (GetActorsInSelectionRectangle<APawn>(StartPos, EndPos, Pawns, true))
	{
		for (auto Pawn : Pawns)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "PAWN!");
			ISelectable* Selectable = Cast<ISelectable>(Pawn);
			if (Selectable)
			{				
				OutSelectablePawns.Add(Pawn);
			}
		}
	}
	PlayerController->SelectUnits(OutSelectablePawns);
}