// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object.h"
#include "UISystem.generated.h"

USTRUCT(BlueprintType)
struct FControlGroupElementBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FText Name;

	UPROPERTY(BlueprintReadWrite)
	FText Description;
};

USTRUCT(BlueprintType)
struct FControlGroupDataBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	bool bGroupEnabled = false;
};

/*----------------Setting group----------------*/

UENUM(BlueprintType)
enum class ESettingType : uint8
{
	Input,
	RangeSlider,
	LabledInput,
	LabledValue
};

USTRUCT(BlueprintType)
struct FGroupElement_Settings : public FControlGroupElementBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	ESettingType Type;

	UPROPERTY(BlueprintReadWrite)
	int Value;

	UPROPERTY(BlueprintReadWrite)
	int MinValue;

	UPROPERTY(BlueprintReadWrite)
	int MaxValue;

};

USTRUCT(BlueprintType)
struct FControlGroupData_Settings : public FControlGroupDataBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TArray<FGroupElement_Settings> SettingsElements;

};

/*----------------Queues group----------------*/

USTRUCT(BlueprintType)
struct FItemElement : public FControlGroupElementBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<class UItem> ItemClass;

	UPROPERTY(BlueprintReadWrite)
	int Count;

};

USTRUCT(BlueprintType)
struct FQueueOutputItem : public FControlGroupElementBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int Index;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* Icon;
};


USTRUCT(BlueprintType)
struct FQueueElement : public FControlGroupElementBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TArray<FItemElement> Inputs;

	UPROPERTY(BlueprintReadWrite)
	int Progress; // 0 - 100

	UPROPERTY(BlueprintReadWrite)
	FQueueOutputItem Output;
};

USTRUCT(BlueprintType)
struct FControlGroupData_Queue : public FControlGroupDataBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FQueueElement> Queues;
};

/*----------------Storage group----------------*/


USTRUCT(BlueprintType)
struct FControlGroupData_Storage : public FControlGroupDataBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FItemElement> StorageElements;
};

/*----------------Trading group----------------*/

USTRUCT(BlueprintType)
struct FControlGroupData_Trading : public FControlGroupDataBase
{
	GENERATED_BODY()
};

/*---------------System----------------------*/


USTRUCT(BlueprintType)
struct FControlData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FControlGroupData_Settings Settings;

	UPROPERTY(BlueprintReadWrite)
	FControlGroupData_Queue Queues;

	UPROPERTY(BlueprintReadWrite)
	FControlGroupData_Storage Storage;

	UPROPERTY(BlueprintReadWrite)
	FControlGroupData_Trading Trading;
	
};