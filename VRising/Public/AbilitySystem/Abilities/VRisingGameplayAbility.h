// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "VRisingGameplayAbility.generated.h"


UCLASS()
class VRISING_API UVRisingGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	UFUNCTION(BlueprintCallable)
	FRotator CalculateInputDirection();
};
