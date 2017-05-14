// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object.h"
#include "Gameplay/Item.h"
#include "GameTypes.generated.h"

USTRUCT(BlueprintType)
struct FItemCeilBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Count;
};

USTRUCT(BlueprintType)
struct FItemCeil : public FItemCeilBase
{
	GENERATED_BODY()

		FItemCeil() : ItemClass(nullptr)
	{
		Count = 0;
	}

	FItemCeil(int num, TSubclassOf<class UItem> item)
	{
		Count = num;
		ItemClass = item;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UItem> ItemClass;

	static FItemCeil EmptyItem;

	/*
	@return left item count
	*/
	int TakeTo(FItemCeil & to, int cnt)
	{
		int left = FMath::Max(0, cnt - Count);
		Count = FMath::Max(0, Count - cnt);
		to.ItemClass = ItemClass;
		to.Count += cnt - left;
		to.Count = FMath::Min(to.Count, GetMaxStack());
		if (Count <= 0) ItemClass = nullptr;
		return left;
	}

	int GetMaxStack() const;

	bool HasEmpty() const
	{
		return ItemClass == nullptr;
	}
};

USTRUCT(BlueprintType)
struct FItemList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	TArray<FItemCeil> Resources;
};

UCLASS(const, Blueprintable, BlueprintType)
class UDamageType_Base : public UDamageType
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "DamageType")
	UTexture2D* DamageIcon;
};


USTRUCT(BlueprintType)
struct FDamage
{
	GENERATED_BODY()

	FDamage();

	UPROPERTY()
	float Min;

	UPROPERTY()
	float Max;

	UPROPERTY()
	FRandomStream Random;

	UPROPERTY()
	TSubclassOf<UDamageType_Base> DamageType;

	UPROPERTY()
	TScriptInterface<class ITeamObjectInterface> Instigator;

	float GetDamage() const;
};

USTRUCT(BlueprintType)
struct FAnimationArray
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimSequenceBase*> Anims;

	FORCEINLINE UAnimSequenceBase* operator[](int index)
	{
		return Anims[index];
	}

	FORCEINLINE int Num() const
	{
		return Anims.Num();
	}
};

UENUM(BlueprintType)
enum class EGameUnitAction : uint8
{
	UA_Chop UMETA(DisplayName = "Chop tree"),
	UA_Mine UMETA(DisplayName = "Mine (Stones, ore)"),
	UA_Dig UMETA(DisplayName = "Dig ground"),
	UA_Collect UMETA(DisplayName = "Collect herbs"),
	UA_Building UMETA(DisplayName = "Building"),
	UA_Attack UMETA(DisplayName = "Attack")
};

USTRUCT(BlueprintType)
struct FResourceItemCeil : public FItemCeilBase
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<class UResourceItem> ItemClass;
};

USTRUCT(BlueprintType)
struct FCommandBase
{
	GENERATED_BODY()

	/*Command Instigator*/
	UPROPERTY()
	AController* Controller = nullptr;
};

USTRUCT(BlueprintType)
struct FCommandTarget: public FCommandBase
{
	GENERATED_BODY()

	UPROPERTY()
	FVector TargetPosition = FVector(0);

	UPROPERTY()
	AActor* TargetActor = nullptr;
};