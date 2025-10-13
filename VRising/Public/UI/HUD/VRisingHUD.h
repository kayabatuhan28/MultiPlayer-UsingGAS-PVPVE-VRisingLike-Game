// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/HUD.h"
#include "VRisingHUD.generated.h"

class UPlayerOverlayWidget;
class UVRisingUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;

UCLASS()
class VRISING_API AVRisingHUD : public AHUD
{
	GENERATED_BODY()

public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	void InıtOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UVRisingUserWidget> OverlayWidget; // Main ana widget olmasi icin olusturduk

	UPROPERTY(EditAnywhere)
	TSubclassOf<UVRisingUserWidget> OverlayWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;



	
	
};
