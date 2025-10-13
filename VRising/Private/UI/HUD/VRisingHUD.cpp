// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/VRisingHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/VRisingUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AVRisingHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallBacksDependencies(); 
	}
	return OverlayWidgetController;
}

void AVRisingHUD::InıtOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// not valid gelirse cöker cökme kisminda özel mesaj yazdirabilmek icin checkf kullanabiliriz.
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));
	
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UVRisingUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	// Baslangic datalarini widgetta setleyebilmek icin (health mana vs) WBP_Overlay widgetinde Event Widget Controller Set kisminda cagirdik
	// Böylece controller setlendiği sirada delegate baglanan mana ve health widgetta progressbari setler.
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

