// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/VRisingWidgetController.h"

#include "AbilitySystem/VRisingAttributeSet.h"
#include "Game/VRisingPlayerController.h"
#include "Game/VRisingPlayerState.h"

void UVRisingWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UVRisingWidgetController::BroadcastInitialValues()
{
	
}

void UVRisingWidgetController::BindCallBacksDependencies()
{
	
}

AVRisingPlayerController* UVRisingWidgetController::GetAuraPC()
{
	if (VRisingPlayerController == nullptr)
	{
		VRisingPlayerController = Cast<AVRisingPlayerController>(PlayerController);
	}
	return VRisingPlayerController;
}

AVRisingPlayerState* UVRisingWidgetController::GetAuraPS()
{
	if (VRisingPlayerState == nullptr)
	{
		VRisingPlayerState = Cast<AVRisingPlayerState>(PlayerState);
	}
	return VRisingPlayerState;
}

UVRisingAbilitySystemComponent* UVRisingWidgetController::GetAuraASC()
{
	if (VRisingAbilitySystemComponent == nullptr)
	{
		VRisingAbilitySystemComponent = Cast<UVRisingAbilitySystemComponent>(AbilitySystemComponent);
	}
	return VRisingAbilitySystemComponent;
}

UVRisingAttributeSet* UVRisingWidgetController::GetAuraAS()
{
	if (VRisingAttributeSet == nullptr)
	{
		VRisingAttributeSet = Cast<UVRisingAttributeSet>(AttributeSet);
	}
	return VRisingAttributeSet;
}
