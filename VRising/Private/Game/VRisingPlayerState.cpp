// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VRisingPlayerState.h"

#include "AbilitySystem/VRisingAbilitySystemComponent.h"
#include "AbilitySystem/VRisingAttributeSet.h"

AVRisingPlayerState::AVRisingPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UVRisingAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UVRisingAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f; // Serverin clientleri ne siklikla updateleyecegini belirler
}

void AVRisingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
}

UAbilitySystemComponent* AVRisingPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
