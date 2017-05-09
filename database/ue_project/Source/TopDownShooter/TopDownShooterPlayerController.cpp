// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TopDownShooter.h"
#include "TopDownShooterPlayerController.h"
#include "TopDownShooterCharacter.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Public/Player/SpectratorCameraPawn.h"
#include "Public/Player/GamePlayerState.h"
#include "Public/HUD/GameHUD.h"
#include "Public/Player/SpectratorCameraComponent.h"
#include "Public/Units/CitizenUnit.h"
#include "Public/Buildings/Building.h"
#include "Public/Buildings/BuildingSampler.h"
#include "UnrealNetwork.h"

ATopDownShooterPlayerController::ATopDownShooterPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;

	IsSamplingBuild = false;
	LastOverlaped = nullptr;
	IsRMouseDown = false;
}

void ATopDownShooterPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATopDownShooterPlayerController, TeamNum);
	DOREPLIFETIME(ATopDownShooterPlayerController, StrategyCameraMode);
}

ATopDownShooterCharacter * ATopDownShooterPlayerController::GetHeroPawn()
{
	return Cast<ATopDownShooterCharacter>(GetPawn());
}

AGameHUD* ATopDownShooterPlayerController::GetGameHUD() const
{
	return Cast<AGameHUD>(GetHUD());
}

AGamePlayerState* ATopDownShooterPlayerController::GetPlayerState() const
{
	return Cast<AGamePlayerState>(PlayerState);
}

void ATopDownShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ABuildingSampler::ClearInstance();
}

void ATopDownShooterPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	ASpectratorCameraPawn* StrategyPawn = Cast<ASpectratorCameraPawn>(GetPawn());
	if (StrategyPawn)
	{
		FVector2D MousePosition;
		const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		const float mouseBorderOffset = 20;
		if (LocalPlayer && LocalPlayer->ViewportClient)
		{
			if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
			{
				FVector2D viewportSize;
				LocalPlayer->ViewportClient->GetViewportSize(viewportSize);

				FVector cameraVelocity(0.0f, 0.0f, 0.0f);
				if (MousePosition.X < mouseBorderOffset)
				{
					//cameraVelocity.Y = -1.0f;
					StrategyPawn->GetStrategyCameraComponent()->MoveRight(-1.0f);
				}

				if (MousePosition.X > viewportSize.X - mouseBorderOffset)
				{
					//cameraVelocity.Y = 1.0f;
					StrategyPawn->GetStrategyCameraComponent()->MoveRight(1.0f);
				}

				if (MousePosition.Y < mouseBorderOffset)
				{
					//cameraVelocity.X = -1.0f;
					StrategyPawn->GetStrategyCameraComponent()->MoveForward(1.0f);
				}

				if (MousePosition.Y > viewportSize.Y - mouseBorderOffset)
				{
					//cameraVelocity.X = 1.0f;
					StrategyPawn->GetStrategyCameraComponent()->MoveForward(-1.0f);
				}
			}
			//StrategyPawn->AddMovementInput(cameraVelocity);
			//StrategyPawn->GetStrategyCameraComponent()->UpdateCameraMovement(this);
		}
		
		if (IsStrategyMode())
		{
			// Strategy mode
			if (IsSamplingBuild && ABuildingSampler::GetInstance(GetWorld()))
			{
				// Setting build location
				if (IsRMouseDown)
				{
					float dx = 0;
					float dy = 0;
					GetInputMouseDelta(dx, dy);
					ABuildingSampler* s = ABuildingSampler::GetInstance(GetWorld());
					s->SetActorRotation(s->GetActorRotation() + FRotator(0.0f, -dx * 20, 0.0f));
				}
				else
				{
					FHitResult Hit;
					GetHitResultUnderCursor(ECC_WorldStatic, true, Hit);
					if (Hit.bBlockingHit && Hit.GetActor())
					{
						ABuildingSampler::GetInstance(GetWorld())->SetActorLocation(Hit.Location);
					}
				}
			}
			else
			{
				
			}
		}
		else
		{
			// Hero mode
		}
	}

	// ---------------Mouse overlaping
	/*FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (Hit.bBlockingHit && Hit.GetActor())
	{
		ITeamObjectInterface* i = Cast<ITeamObjectInterface>(Hit.GetActor());
		if (i)
		{
			if (i != LastOverlaped)
			{
				if (LastOverlaped != nullptr)
					LastOverlaped->Execute_OnMouseOut(this);
				i->Execute_OnMouseIn(this);
			}
		}
		else if (LastOverlaped != nullptr)
		{
			LastOverlaped->Execute_OnMouseOut(this);
			LastOverlaped = nullptr;
		}
	}
	else if (LastOverlaped != nullptr)
	{
		LastOverlaped->Execute_OnMouseOut(this);
		LastOverlaped = nullptr;
	}*/
	//------------------------------------
}

void ATopDownShooterPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveUp", this, &ATopDownShooterPlayerController::OnMoveUp);
	InputComponent->BindAxis("MoveRight", this, &ATopDownShooterPlayerController::OnMoveRight);

	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATopDownShooterPlayerController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &ATopDownShooterPlayerController::OnSetDestinationReleased);

	//InputComponent->BindAction("Attack", IE_Pressed, this, &ATopDownShooterPlayerController::OnAttackPressed);
	//InputComponent->BindAction("Attack", IE_Released, this, &ATopDownShooterPlayerController::OnAttackReleased);

	InputComponent->BindAction("Inventory", IE_Pressed, this, &ATopDownShooterPlayerController::OnOpenInventoryPressed);
	InputComponent->BindAction("Inventory", IE_Released, this, &ATopDownShooterPlayerController::OnOpenInventoryReleased);

	InputComponent->BindAction("ToggleMode", IE_Pressed, this, &ATopDownShooterPlayerController::OnSetToggleModePressed);

	InputComponent->BindAction("LMouse", IE_Pressed, this, &ATopDownShooterPlayerController::OnLMouseDownPressed);
	InputComponent->BindAction("LMouse", IE_Released, this, &ATopDownShooterPlayerController::OnLMouseDownReleased);

	InputComponent->BindAction("RMouse", IE_Pressed, this, &ATopDownShooterPlayerController::OnRMouseDownPressed);
	InputComponent->BindAction("RMouse", IE_Released, this, &ATopDownShooterPlayerController::OnRMouseDownReleased);

	// support touch devices 
	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATopDownShooterPlayerController::MoveToTouchLocation);
	//InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATopDownShooterPlayerController::MoveToTouchLocation);
}

void ATopDownShooterPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void ATopDownShooterPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ATopDownShooterPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const Pawn = GetHeroPawn();
	if (Pawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ATopDownShooterPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ATopDownShooterPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void ATopDownShooterPlayerController::OnAttackPressed()
{
	
}

void ATopDownShooterPlayerController::OnAttackReleased()
{

}

void ATopDownShooterPlayerController::OnMoveUp(float rate)
{	

}

void ATopDownShooterPlayerController::OnMoveRight(float rate)
{

}

void ATopDownShooterPlayerController::OnSetToggleModePressed()
{
	ChangeMode();
}

void ATopDownShooterPlayerController::OnRep_CamMode()
{
	
}

bool ATopDownShooterPlayerController::ChangeMode_Validate()
{
	return true;
}

void ATopDownShooterPlayerController::ChangeMode_Implementation()
{

}

void ATopDownShooterPlayerController::ClientChangeMode_Implementation()
{

}

bool ATopDownShooterPlayerController::MakeBuilding_Validate(FVector Location, FRotator Rotation, TSubclassOf<ABuilding> BuildingClass)
{
	return true;
}

void ATopDownShooterPlayerController::MakeBuilding_Implementation(FVector Location, FRotator Rotation, TSubclassOf<ABuilding> BuildingClass)
{
	ABuilding* building = GetWorld()->SpawnActor<ABuilding>(BuildingClass);
	if (building)
	{
		building->SetActorLocation(Location);
		building->SetActorRotation(Rotation);
		building->SetTeamNum(TeamNum);
		building->SetOwner(this);
	}
}

void ATopDownShooterPlayerController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);
	ATopDownShooterCharacter* ch = Cast<ATopDownShooterCharacter>(aPawn);
	if (ch)
	{
		HeroCharacter = ch;
	}
}

void ATopDownShooterPlayerController::OnOpenInventoryPressed()
{
	if (!StrategyCameraMode && GetGameHUD())
	{
		GetGameHUD()->OnToggleHeroInventory();
	}
}

void ATopDownShooterPlayerController::OnOpenInventoryReleased()
{

}

void ATopDownShooterPlayerController::OnLMouseDownPressed()
{
	if (IsStrategyMode())
	{
		if (IsSamplingBuild && ABuildingSampler::GetInstance(GetWorld()))
		{
			/*FHitResult HitResult;
			GetHitResultUnderCursor(ECC_WorldStatic, false, HitResult);
			if (HitResult.bBlockingHit && HitResult.Actor.IsValid())
			{
				MakeBuilding(HitResult.Location, ABuildingSampler::GetInstance(GetWorld())->BuildingClass);
				EndSampleBuilding();
			}*/
			MakeBuilding(ABuildingSampler::GetInstance(GetWorld())->GetActorLocation(),
						ABuildingSampler::GetInstance(GetWorld())->GetActorRotation(),
						ABuildingSampler::GetInstance(GetWorld())->BuildingClass);
			EndSampleBuilding();
		}
		else 
		{
			FHitResult HitResult;
			GetHitResultUnderCursor(ECC_Pawn, false, HitResult);
			if (HitResult.Actor.IsValid())
			{

			}
			if (HitResult.Actor.IsValid() && Cast<ACitizenUnit>(HitResult.Actor.Get()))
			{
				SelectedUnit = Cast<ACitizenUnit>(HitResult.Actor.Get());
			}
		}
	}
}

void ATopDownShooterPlayerController::OnLMouseDownReleased()
{
	
}

void ATopDownShooterPlayerController::OnRMouseDownPressed()
{
	IsRMouseDown = true;
	/*if (GetHeroPawn() != nullptr && GetStateName() != NAME_Spectating)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString("BeginAttack"));
			GetHeroPawn()->BeginAttack(Hit.ImpactPoint);
		}
	}
	if (IsStrategyMode())
	{

	}*/
}

void ATopDownShooterPlayerController::OnRMouseDownReleased()
{
	IsRMouseDown = false;
}

bool ATopDownShooterPlayerController::IsStrategyMode()
{
	return GetStateName() == NAME_Spectating;
}

void ATopDownShooterPlayerController::BeginSampleBuilding(TSubclassOf<ABuilding> BuildingClass)
{
	ABuildingSampler* sampler = ABuildingSampler::GetInstance(GetWorld());
	if (IsStrategyMode() && sampler)
	{
		IsSamplingBuild = true;
		ABuilding* def = BuildingClass.GetDefaultObject();
		if(def)
			sampler->SetMesh(def->GetBuildingMesh());
		sampler->BuildingClass = BuildingClass;
		sampler->SetActorHiddenInGame(false);
	}
}

void ATopDownShooterPlayerController::EndSampleBuilding()
{
	IsSamplingBuild = false;
	if (ABuildingSampler::GetInstance(GetWorld()))
	{
		ABuildingSampler::GetInstance(GetWorld())->SetActorHiddenInGame(true);
	}
}