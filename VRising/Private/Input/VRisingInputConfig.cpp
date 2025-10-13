// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/VRisingInputConfig.h"

const UInputAction* UVRisingInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FVRisingInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"),*InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
