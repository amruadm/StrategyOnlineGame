// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object.h"
#include "MathHelpers.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UMathHelpers: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//Return value between 0.0f and 1.0f
	static float Noise(int x, int y, FRandomStream & RandStream);
};
