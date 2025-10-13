// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/WeaponChangeInfo.h"
#include "UI/WidgetController/VRisingWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UVRisingUserWidget;
class UVRisingAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDieSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangedSignature, EWeaponType, NewWeapon, UWeaponChangeInfo*, WeaponInfo);

UCLASS(BlueprintType,  Blueprintable)
class VRISING_API UOverlayWidgetController : public UVRisingWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override; 

	// Gastan, abilitycomptan gelen AttributeChanged delegateleri baglamak icin kullandik, AuraHud.cpp de kullandik.
	virtual void BindCallBacksDependencies() override; 

	// Health Globe widgetlarında bu delegateleri bağladık, bu delegateleride globe widgetlarda assignlariz
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponChangedSignature OnWeaponChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDieSignature OnPlayerDie;

	
};
