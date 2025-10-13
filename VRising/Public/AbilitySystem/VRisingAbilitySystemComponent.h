// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "VRisingAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

UCLASS()
class VRISING_API UVRisingAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	// Abilityi karaktere verdigimiz zaman bu delegate ile Skillerin oldugu widgeta bunun bilgisini göndermek icin ekledik.
	FAbilitiesGiven AbilitiesGivenDelegate;

	void AbilityActorInfoSet();

	// Bu silinebilir ability silip yeni ability ekleme testi için
	UFUNCTION(BlueprintCallable)
	void TestAddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> & StartupAbilities);

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> & StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> & StartupPassiveAbilities);

	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ForEachAbility(const FForEachAbility& Delegate);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

protected:

	virtual void OnRep_ActivateAbilities() override;
	
};
