// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_Paladin.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "GameplayTagsManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/VRisingEnemyCharacter.h"


UBTService_Paladin::UBTService_Paladin(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Paladin Condition Check";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTService_Paladin::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_Paladin::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn())
	{
		AVRisingEnemyCharacter* AiRef = Cast<AVRisingEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
		if (AiRef && AiRef->bDead == false)
		{
			if (AiRef->FocusedTarget)
			{
				if (AiRef->IsPhase2Activated == false)
				{
					if (CheckCanCastSkill(AiRef) == true)
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",3);
					}
					else if (CheckCanLightAttack(AiRef) == true)
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",2);
					}
					else if (AiRef->GetDistanceTo(AiRef->FocusedTarget) >= AiRef->StrafeDistance) 
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",0);
					}
					else if (AiRef->GetDistanceTo(AiRef->FocusedTarget) < AiRef->StrafeDistance && CheckCanStrafe(AiRef))
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",1);
					}
				}
				else // TODO : Phase 2 states
				{
					if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("Phase2StartCombat") == true)
					{
						if (CheckCanCastSkill(AiRef) == true)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",3);
						}
						else if (CheckCanLightAttack(AiRef) == true)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",2);
						}
						else if (AiRef->GetDistanceTo(AiRef->FocusedTarget) >= AiRef->StrafeDistance) 
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",0);
						}
						else if (AiRef->GetDistanceTo(AiRef->FocusedTarget) < AiRef->StrafeDistance && CheckCanStrafe(AiRef))
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",1);
						}
					}
				}
			}
		}
	}
}

bool UBTService_Paladin::CheckCanLightAttack(AVRisingEnemyCharacter* OwnerAi)
{
	// AbilitySystem.Cooldown.EnemyLightAttack
	FGameplayTag LightAttackCooldoownTag = UGameplayTagsManager::Get().RequestGameplayTag("AbilitySystem.Cooldown.EnemyLightAttack");
	FGameplayTag AttackCooldownTag = UGameplayTagsManager::Get().RequestGameplayTag("AbilitySystem.Status.AttackCd");
	if(
		OwnerAi->GetAbilitySystemComponent()->HasMatchingGameplayTag(LightAttackCooldoownTag) == false &&
		OwnerAi->GetAbilitySystemComponent()->HasMatchingGameplayTag(AttackCooldownTag) == false &&
		OwnerAi->GetDistanceTo(OwnerAi->FocusedTarget) <= OwnerAi->AttackDistance)
	{
		return true;
	}

	return false;
}

bool UBTService_Paladin::CheckCanCastSkill(AVRisingEnemyCharacter* OwnerAi)
{
	// AbilitySystem.Cooldown.EnemySkill
	FGameplayTag SkillCooldoownTag = UGameplayTagsManager::Get().RequestGameplayTag("AbilitySystem.Cooldown.EnemySkill");
	FGameplayTag AttackCooldownTag = UGameplayTagsManager::Get().RequestGameplayTag("AbilitySystem.Status.AttackCd");
	if
	(
		OwnerAi->GetAbilitySystemComponent()->HasMatchingGameplayTag(SkillCooldoownTag) == false &&
		OwnerAi->GetAbilitySystemComponent()->HasMatchingGameplayTag(AttackCooldownTag) == false &&
		OwnerAi->bIsCombatModeActivated == true )
	{
		return true;
	}

	return false;
}

bool UBTService_Paladin::CheckCanDodge(AVRisingEnemyCharacter* OwnerAi)
{
	// AbilitySystem.Cooldown.Dodge
	return false;
}

bool UBTService_Paladin::CheckCanStrafe(AVRisingEnemyCharacter* OwnerAi)
{
	FGameplayTag LightAttackCooldoownTag = UGameplayTagsManager::Get().RequestGameplayTag("AbilitySystem.Abilities.EnemyLightAttack");
	FGameplayTag SkillCooldoownTag = UGameplayTagsManager::Get().RequestGameplayTag("AbilitySystem.Abilities.EnemySkillAttack");
	if
	(
		OwnerAi->GetAbilitySystemComponent()->HasMatchingGameplayTag(LightAttackCooldoownTag) == true ||
		OwnerAi->GetAbilitySystemComponent()->HasMatchingGameplayTag(SkillCooldoownTag) == true )
	{
		return false;
	}

	return true;
}
