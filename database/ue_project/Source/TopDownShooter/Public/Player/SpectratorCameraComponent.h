// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "SpectratorCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API USpectratorCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
		USpectratorCameraComponent();

public:
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	/** The minimum offset of the camera. */
	float MinCameraOffset;

	/** The maximum offset of the camera. */
	float MaxCameraOffset;

	FRotator FixedCameraAngle;

	/** If set, camera position will be clamped to movement bounds. */
	uint8 bShouldClampCamera : 1;

	/** Bounds for camera movement. */
	FBox CameraMovementBounds;

	/** Percentage of minimap where center of camera can be placed. */
	float MiniMapBoundsLimit;

	/** Viewport size associated with camera bounds. */
	FVector2D CameraMovementViewportSize;

	/** Size of the area at the edge of the screen that will trigger camera scrolling. */
	uint32 CameraActiveBorder;

	/** Handle zooming in. */
	void OnZoomIn();

	/** Handle zooming out. */
	void OnZoomOut();

	/*
	* Move the camera on the forward axis
	*
	* @param	Val		Amount to move
	*/
	void MoveForward(float Val);

	/*
	* Move the camera on the left/right axis
	*
	* @param	Val		Amount to move
	*/
	void MoveRight(float Val);

	void UpdateCameraMovement(const APlayerController* InPlayerController);

	/** Sets the desired zoom level; clamping if necessary */
	void SetZoomLevel(float NewLevel);

	/*
	* CLamp the Camera location.
	*
	* @param	InPlayerController	The player controller relative to this component.
	* @param	OutCameraLocation	Structure to receive the clamped coordinates.
	*/
	void ClampCameraLocation(const APlayerController* InPlayerController, FVector& OutCameraLocation);

	void AddNoScrollZone(FBox InCoords);

private:

	/* Update the movement bounds of this component. */
	void UpdateCameraBounds(const APlayerController* InPlayerController);

	/** Return the pawn that owns this component. */
	APawn* GetOwnerPawn();

	/** Return the player controller of the pawn that owns this component. */
	APlayerController* GetPlayerController();

	/* List of zones to exclude from scrolling during the camera movement update. */
	TArray<FBox>	NoScrollZones;

	/** Current amount of camera zoom. */
	float ZoomAlpha;

	float CameraScrollSpeed;
	float MinZoomLevel;
	float MaxZoomLevel;
	
	
};
