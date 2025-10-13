// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/VRisingUserWidget.h"
#include "PlayerOnHeadWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerStatusChangedSignature, bool, bIsTagAdded);

UCLASS()
class VRISING_API UPlayerOnHeadWidget : public UVRisingUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerAttributeChangedSignature OnPlayerHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerAttributeChangedSignature OnPlayerMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerAttributeChangedSignature OnPlayerManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerAttributeChangedSignature OnPlayerMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Status")
	FPlayerStatusChangedSignature OnPlayerStunned;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Status")
	FPlayerStatusChangedSignature OnPlayerSlowed;
	
};
