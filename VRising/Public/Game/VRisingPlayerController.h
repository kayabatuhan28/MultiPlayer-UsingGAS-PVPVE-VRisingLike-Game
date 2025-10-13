// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "AbilitySystem/VRisingAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "VRisingPlayerController.generated.h"


class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UVRisingInputConfig;
class UPlayerOverlayWidget;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AVRisingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVRisingPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Movement Value")
	float ActionValueX;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Movement Value")
	float ActionValueY;

protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> VRisingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_EquipWeapon1;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_EquipWeapon2;
	
	void Move(const FInputActionValue& InputActionValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UVRisingInputConfig> InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UVRisingAbilitySystemComponent* GetASC();

	UPROPERTY()
	TObjectPtr<UVRisingAbilitySystemComponent> VRisingAbilitySystemComponent;
	
};


