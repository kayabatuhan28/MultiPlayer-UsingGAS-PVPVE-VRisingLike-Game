// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/VRisingAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/VRisingBaseCharacter.h"
#include "Character/VRisingEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Net/UnrealNetwork.h"

UVRisingAttributeSet::UVRisingAttributeSet()
{
	
}

void UVRisingAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UVRisingAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVRisingAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UVRisingAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVRisingAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UVRisingAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVRisingAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVRisingAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
}

void UVRisingAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	
	
}

void UVRisingAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data,Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		
		const float DamageReduction = (LocalIncomingDamage / 100.f) * GetArmor();
		LocalIncomingDamage -= DamageReduction;

		if (Props.SourceAvatarActor->ActorHasTag("PvpEnabled"))
		{
			LocalIncomingDamage /= 2.f;
		}

		if (Props.SourceASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("AbilitySystem.Effects.Shielded")))
		{
			LocalIncomingDamage = 0.f;
		}
		
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f; 
			if (bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
			}
			
			ShowFloatingText(Props, LocalIncomingDamage, false);
		}

		if (Props.SourceAvatarActor->ActorHasTag("Paladin") && GetHealth() <= GetMaxHealth() / 2.f)
		{
			if (AVRisingEnemyCharacter* BossRef = Cast<AVRisingEnemyCharacter>(Props.SourceAvatarActor))
			{
				BossRef->Phase2Activate();
			}
		}

		
	}

	if (Data.EvaluatedData.Attribute == GetIncomingHealAttribute())
	{
		float LocalIncomingHeal = GetIncomingHeal();
		SetIncomingHeal(0.f);

		if (Props.SourceAvatarActor->ActorHasTag("PvpEnabled"))
		{
			LocalIncomingHeal /= 2.f;
		}
		
		if (LocalIncomingHeal > 0.f)
		{
			const float NewHealth = GetHealth() + LocalIncomingHeal;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			ShowFloatingText(Props, LocalIncomingHeal, true);
		}
	}

	
}

void UVRisingAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bIsHealing) const
{
	if (AVRisingBaseCharacter* BaseCharacter = Cast<AVRisingBaseCharacter>(Props.SourceCharacter))
	{
		BaseCharacter->ShowDamageNumber(Damage, Props.TargetCharacter, bIsHealing);
	}
}

void UVRisingAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	
}



void UVRisingAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
                                               FEffectProperties& Props) const
{
	// Source : Effecte sebep olan, Target : Effectten etkilenen (Bu AttributeSetin Owneri, yani bu class)
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}

		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
	
}



void UVRisingAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVRisingAttributeSet, Health, OldHealth);
}

void UVRisingAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVRisingAttributeSet, Mana, OldMana);
}

void UVRisingAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVRisingAttributeSet, MaxHealth, OldMaxHealth);
}

void UVRisingAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVRisingAttributeSet, MaxMana, OldMaxMana);
}

void UVRisingAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVRisingAttributeSet, Armor, OldArmor);
}

void UVRisingAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVRisingAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UVRisingAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVRisingAttributeSet, ManaRegeneration, OldManaRegeneration);
}
