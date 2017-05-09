// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "IslandLevel.generated.h"


USTRUCT(Blueprintable)
struct FIslandLevelMap
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, Category = Map)
		UTexture2D * textureMap;
	FUpdateTextureRegion2D* mUpdateTextureRegion;
	uint8* mDynamicColors;
	float* mDynamicColorsFloat;
	uint32 mDataSize;
	uint32 mDataSqrtSize;

	//void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);
};


/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UIslandLevel : public UObject
{
	GENERATED_BODY()
	
public:

	UIslandLevel();

	UPROPERTY(BlueprintReadWrite, Category = level)
		FIslandLevelMap levelMap;

	UFUNCTION(BlueprintCallable, Category = Generation)
		virtual void GenerateLevel();	
};
