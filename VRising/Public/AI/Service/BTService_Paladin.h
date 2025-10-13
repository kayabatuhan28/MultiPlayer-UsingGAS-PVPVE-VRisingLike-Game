// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_Paladin.generated.h"

class AVRisingEnemyCharacter;

UCLASS()
class VRISING_API UBTService_Paladin : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:

	UBTService_Paladin(const FObjectInitializer& ObjectInitializer);	
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool CheckCanLightAttack(AVRisingEnemyCharacter* OwnerAi);

	bool CheckCanCastSkill(AVRisingEnemyCharacter* OwnerAi);

	bool CheckCanDodge(AVRisingEnemyCharacter* OwnerAi);

	bool CheckCanStrafe(AVRisingEnemyCharacter* OwnerAi);

	// Paladin States
	// 0 : Chase
	// 1 : Strafe
	// 2 : LightAttack
	// 3 : Skill Attack
	// 4 : Dodge
	
};
