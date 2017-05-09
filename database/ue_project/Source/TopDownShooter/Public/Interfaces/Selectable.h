// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Selectable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USelectable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class TOPDOWNSHOOTER_API ISelectable
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, Category = "Selectable")
	void Selected();

	UFUNCTION(BlueprintNativeEvent, Category = "Selectable")
	void Unselected();
};
