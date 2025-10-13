// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/VRisingUserWidget.h"

void UVRisingUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
