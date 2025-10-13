// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/VRisingUserWidget.h"
#include "EnemyOnHeadWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyAttributeChangedSignature, float, NewValue);


UCLASS()
class VRISING_API UEnemyOnHeadWidget : public UVRisingUserWidget
{
	GENERATED_BODY()

public:

	// Attribute Bind
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnEnemyAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnEnemyAttributeChangedSignature OnMaxHealthChanged;

	UFUNCTION(BlueprintImplementableEvent)
	void SetEnemyName(const FString& Input);


	
};
