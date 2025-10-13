// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_BasicEnemy.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "GameplayTagsManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/VRisingEnemyCharacter.h"

UBTService_BasicEnemy::UBTService_BasicEnemy(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Basic Enemy Condition Check";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTService_BasicEnemy::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_BasicEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn())
	{
		AVRisingEnemyCharacter* AiRef = Cast<AVRisingEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
		if (AiRef && AiRef->bDead == false)
		{
			if (AiRef->FocusedTarget)
			{
				if (AiRef->GetDistanceTo(AiRef->FocusedTarget) >= AiRef->StrafeDistance) // Set State Chase
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",0);
				}
				else if (AiRef->GetDistanceTo(AiRef->FocusedTarget) < AiRef->StrafeDistance)
				{
					if (AiRef->GetDistanceTo(AiRef->FocusedTarget) <= AiRef->AttackDistance && CheckCanAttack(AiRef)) // Set State Attack
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",2);
					}
					else // Set State Strafe
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State",1);
					}
				}
			}
		}
	}
}

bool UBTService_BasicEnemy::CheckCanAttack(AVRisingEnemyCharacter* OwnerAi)
{
	FGameplayTag CooldoownTag = UGameplayTagsManager::Get().RequestGameplayTag("AbilitySystem.Cooldown.EnemyLightAttack");
	FGameplayTag NormalHitReactTag = UGameplayTagsManager::Get().RequestGameplayTag("AbilitySystem.Abilities.NormalHitReact");
	FGameplayTag StunTag = UGameplayTagsManager::Get().RequestGameplayTag("AbilitySystem.Abilities.Stun");
	if
	(
		OwnerAi->GetAbilitySystemComponent()->HasMatchingGameplayTag(CooldoownTag) ||
		OwnerAi->GetAbilitySystemComponent()->HasMatchingGameplayTag(NormalHitReactTag) ||
		OwnerAi->GetAbilitySystemComponent()->HasMatchingGameplayTag(StunTag))
	{
		return false;
	}

	return true;
}

