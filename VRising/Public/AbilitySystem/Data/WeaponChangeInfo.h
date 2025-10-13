// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponChangeInfo.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Default,
	GreatAxe,
	Staff,
	Bow
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class Defaults")
	TArray<UTexture2D*> SkillIcon = {};

	
};

UCLASS()
class VRISING_API UWeaponChangeInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main")
	TArray<FWeaponData> WeaponInformation;
	
};
