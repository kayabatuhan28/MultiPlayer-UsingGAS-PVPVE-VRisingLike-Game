// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/VRisingPlayerController.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Character/VRisingCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Input/VRisingInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AVRisingPlayerController::AVRisingPlayerController()
{
	
}

void AVRisingPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVRisingPlayerController, ActionValueX);
	DOREPLIFETIME(AVRisingPlayerController, ActionValueY);
}

void AVRisingPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(VRisingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
}


void AVRisingPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UVRisingInputComponent* VRisingInputComponent = CastChecked<UVRisingInputComponent>(InputComponent);

	VRisingInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVRisingPlayerController::Move);
	
	VRisingInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AVRisingPlayerController::Move(const FInputActionValue& InputActionValue)
{
	/*
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>(); 
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	*/
}

void AVRisingPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	
}

void AVRisingPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
}

void AVRisingPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}

UVRisingAbilitySystemComponent* AVRisingPlayerController::GetASC()
{
	if (VRisingAbilitySystemComponent == nullptr)
	{
		VRisingAbilitySystemComponent = Cast<UVRisingAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return VRisingAbilitySystemComponent;
}


