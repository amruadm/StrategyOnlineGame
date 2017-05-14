// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Player/SpectratorCameraComponent.h"
#include "Public/Player/SpectratorMovementComponent.h"


USpectratorCameraComponent::USpectratorCameraComponent()
{
	ZoomAlpha = 1.0f;
	CameraScrollSpeed = 4000.0f;
	MinZoomLevel = 0.4f;
	MaxZoomLevel = 1.0f;
	MiniMapBoundsLimit = 0.8f;
	FixedCameraAngle = FRotator(-45.0f, 0.0f, 0.0f);
	MaxCameraOffset = 2000.0f;
	MinCameraOffset = 1400.0f;
}

void USpectratorCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);
	APlayerController* Controller = GetPlayerController();
	if (Controller)
	{
		DesiredView.FOV = 50.f;
		const float CurrentOffset = MinCameraOffset + ZoomAlpha * (MaxCameraOffset - MinCameraOffset);
		FVector Pos2 = Controller->GetFocalLocation();
		DesiredView.Location = Controller->GetFocalLocation() - FixedCameraAngle.Vector() * CurrentOffset;
		DesiredView.Rotation = FixedCameraAngle;
	}
}

void USpectratorCameraComponent::MoveForward(float Val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn != NULL)
	{
		APlayerController* Controller = GetPlayerController();
		if ((Val != 0.f) && (Controller != NULL))
		{
			FRotator rot = Controller->PlayerCameraManager->GetCameraRotation();
			rot.Pitch = 0.0f;
			rot.Roll = 0.0f;

			const FRotationMatrix R(rot);
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::X) * 100.0f;

			// transform to world space and add it
			OwnerPawn->AddMovementInput(WorldSpaceAccel, Val);
		}
	}
}

void USpectratorCameraComponent::MoveRight(float Val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn != NULL)
	{
		APlayerController* Controller = GetPlayerController();
		if ((Val != 0.f) && (Controller != NULL))
		{
			FRotator rot = Controller->PlayerCameraManager->GetCameraRotation();
			rot.Pitch = 0.0f;
			rot.Roll = 0.0f;

			const FRotationMatrix R(rot);
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::Y) * 100.0f;

			// transform to world space and add it
			OwnerPawn->AddMovementInput(WorldSpaceAccel, Val);
		}
	}
}

/** Return the pawn that owns this component. */
APawn* USpectratorCameraComponent::GetOwnerPawn()
{
	return Cast<APawn>(GetOwner());
}

/** Return the player controller of the pawn that owns this component. */
APlayerController* USpectratorCameraComponent::GetPlayerController()
{
	APlayerController* Controller = NULL;
	APawn* Owner = GetOwnerPawn();
	if (Owner != NULL)
	{
		Controller = Cast<APlayerController>(Owner->GetController());
	}
	return Controller;
}

/** Handle zooming in. */
void USpectratorCameraComponent::OnZoomIn()
{
	SetZoomLevel(ZoomAlpha - 0.1f);
}

/** Handle zooming out. */
void USpectratorCameraComponent::OnZoomOut()
{
	SetZoomLevel(ZoomAlpha + 0.1f);
}

/** Sets the desired zoom level; clamping if necessary */
void USpectratorCameraComponent::SetZoomLevel(float NewLevel)
{
	ZoomAlpha = FMath::Clamp(NewLevel, MinZoomLevel, MaxZoomLevel);
}

void USpectratorCameraComponent::ClampCameraLocation(const APlayerController* InPlayerController, FVector& OutCameraLocation)
{
	if (bShouldClampCamera)
	{
		UpdateCameraBounds(InPlayerController);
		if (CameraMovementBounds.GetSize() != FVector::ZeroVector)
		{
			OutCameraLocation = CameraMovementBounds.GetClosestPointTo(OutCameraLocation);
		}
	}
}

void USpectratorCameraComponent::AddNoScrollZone(FBox InCoords)
{
	NoScrollZones.AddUnique(InCoords);
}

void USpectratorCameraComponent::UpdateCameraBounds(const APlayerController* InPlayerController)
{
	ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(InPlayerController->Player);
	if (LocalPlayer == NULL || LocalPlayer->ViewportClient == NULL)
	{
		return;
	}

	FVector2D CurrentViewportSize;
	LocalPlayer->ViewportClient->GetViewportSize(CurrentViewportSize);

	// calc frustum edge direction, from bottom left corner
	if (CameraMovementBounds.GetSize() == FVector::ZeroVector || CurrentViewportSize != CameraMovementViewportSize)
	{
		// calc frustum edge direction, from bottom left corner
		const FVector FrustumRay2DDir = FVector(1, 1, 0).GetSafeNormal();
		const FVector FrustumRay2DRight = FVector::CrossProduct(FrustumRay2DDir, FVector::UpVector);
		const FQuat RotQuat(FrustumRay2DRight, FMath::DegreesToRadians(90.0f - InPlayerController->PlayerCameraManager->GetFOVAngle() * 0.5f));
		const FVector FrustumRayDir = RotQuat.RotateVector(FrustumRay2DDir);

		// collect 3 world bounds' points and matching frustum rays (bottom left, top left, bottom right)
		FBox Bounds;
		//if (MyGameState)
		//{
			//FBox const& WorldBounds = MyGameState->WorldBounds;
		FBox const& WorldBounds = Bounds;

			if (WorldBounds.GetSize() != FVector::ZeroVector)
			{
				const FVector WorldBoundPoints[] = {
					FVector(WorldBounds.Min.X, WorldBounds.Min.Y, WorldBounds.Max.Z),
					FVector(WorldBounds.Min.X, WorldBounds.Max.Y, WorldBounds.Max.Z),
					FVector(WorldBounds.Max.X, WorldBounds.Min.Y, WorldBounds.Max.Z)
				};
				const FVector FrustumRays[] = {
					FVector(FrustumRayDir.X, FrustumRayDir.Y, FrustumRayDir.Z),
					FVector(FrustumRayDir.X, -FrustumRayDir.Y, FrustumRayDir.Z),
					FVector(-FrustumRayDir.X, FrustumRayDir.Y, FrustumRayDir.Z)
				};

				// get camera plane for intersections
				const FPlane CameraPlane = FPlane(InPlayerController->GetFocalLocation(), FVector::UpVector);

				// get matching points on camera plane
				const FVector CameraPlanePoints[3] = {
					UGameHelpers::IntersectRayWithPlane(WorldBoundPoints[0], FrustumRays[0], CameraPlane)*MiniMapBoundsLimit,
					UGameHelpers::IntersectRayWithPlane(WorldBoundPoints[1], FrustumRays[1], CameraPlane)*MiniMapBoundsLimit,
					UGameHelpers::IntersectRayWithPlane(WorldBoundPoints[2], FrustumRays[2], CameraPlane)*MiniMapBoundsLimit
				};

				// create new bounds
				CameraMovementBounds = FBox(CameraPlanePoints, 3);
				CameraMovementViewportSize = CurrentViewportSize;
			}
		//}
	}
}



void USpectratorCameraComponent::UpdateCameraMovement(const APlayerController* InPlayerController)
{
	// No mouse support on mobile
#if PLATFORM_DESKTOP
	ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(InPlayerController->Player);
	if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
	{
		FVector2D MousePosition;
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition) == false)
		{
			return;
		}

		FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
		const float ScrollSpeed = 60.0f;
		const FIntPoint ViewportSize = Viewport->GetSizeXY();
		const uint32 ViewLeft = FMath::TruncToInt(LocalPlayer->Origin.X * ViewportSize.X);
		const uint32 ViewRight = ViewLeft + FMath::TruncToInt(LocalPlayer->Size.X * ViewportSize.X);
		const uint32 ViewTop = FMath::TruncToInt(LocalPlayer->Origin.Y * ViewportSize.Y);
		const uint32 ViewBottom = ViewTop + FMath::TruncToInt(LocalPlayer->Size.Y * ViewportSize.Y);

		const float MaxSpeed = CameraScrollSpeed * FMath::Clamp(ZoomAlpha, 0.3f, 1.0f);

		//AMainGameState const* const MyGameState = GetWorld()->GetGameState<AMainGameState >();
		bool bNoScrollZone = false;
		FVector MouseCoords(MousePosition, 0.0f);
		for (int iZone = 0; iZone < NoScrollZones.Num(); iZone++)
		{
			FBox EachZone = NoScrollZones[iZone];
			if (EachZone.IsInsideXY(MouseCoords) == true)
			{
				bNoScrollZone = true;
			}
		}

		const uint32 MouseX = MousePosition.X;
		const uint32 MouseY = MousePosition.Y;
		float SpectatorCameraSpeed = MaxSpeed;
		ASpectatorPawn* SpectatorPawn = NULL;
		if (GetPlayerController() != NULL)
		{
			SpectatorPawn = GetPlayerController()->GetSpectatorPawn();
			if (SpectatorPawn->GetMovementComponent() != NULL)
			{
				SpectatorCameraSpeed = GetDefault<USpectratorMovementComponent>(SpectatorPawn->GetMovementComponent()->GetClass())->MaxSpeed;
			}
		}
		if (!bNoScrollZone)
		{
			if (MouseX >= ViewLeft && MouseX <= (ViewLeft + CameraActiveBorder))
			{
				const float delta = 1.0f - float(MouseX - ViewLeft) / CameraActiveBorder;
				SpectatorCameraSpeed = delta * MaxSpeed;
				MoveRight(-ScrollSpeed * delta);
			}
			else if (MouseX >= (ViewRight - CameraActiveBorder) && MouseX <= ViewRight)
			{
				const float delta = float(MouseX - ViewRight + CameraActiveBorder) / CameraActiveBorder;
				SpectatorCameraSpeed = delta * MaxSpeed;
				MoveRight(ScrollSpeed * delta);
			}

			if (MouseY >= ViewTop && MouseY <= (ViewTop + CameraActiveBorder))
			{
				const float delta = 1.0f - float(MouseY - ViewTop) / CameraActiveBorder;
				SpectatorCameraSpeed = delta * MaxSpeed;
				MoveForward(ScrollSpeed * delta);
			}
			else if (MouseY >= (ViewBottom - CameraActiveBorder) && MouseY <= ViewBottom)
			{
				const float delta = float(MouseY - (ViewBottom - CameraActiveBorder)) / CameraActiveBorder;
				SpectatorCameraSpeed = delta * MaxSpeed;
				MoveForward(-ScrollSpeed * delta);
			}

			if (SpectatorPawn != NULL)
			{
				UFloatingPawnMovement* PawnMovementComponent = Cast<UFloatingPawnMovement>(SpectatorPawn->GetMovementComponent());
				if (PawnMovementComponent)
				{
					PawnMovementComponent->MaxSpeed = SpectatorCameraSpeed;
				}
			}
		}
	}
#endif
	NoScrollZones.Empty();
}