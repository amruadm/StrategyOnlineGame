// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "CommonHelpers.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UCommonHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

};
