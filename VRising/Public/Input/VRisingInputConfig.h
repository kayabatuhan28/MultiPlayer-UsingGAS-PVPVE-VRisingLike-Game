// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "VRisingInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FVRisingInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

UCLASS()
class VRISING_API UVRisingInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	// AbilityInputActions struct arrayinde gezer input olarak verilen tagi arar bulursa onun input actionu returnler
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FVRisingInputAction> AbilityInputActions;
	
	
};
