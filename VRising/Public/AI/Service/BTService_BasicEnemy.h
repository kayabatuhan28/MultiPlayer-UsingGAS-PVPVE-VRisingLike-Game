// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_BasicEnemy.generated.h"

class AVRisingEnemyCharacter;

UCLASS()
class VRISING_API UBTService_BasicEnemy : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:

	UBTService_BasicEnemy(const FObjectInitializer& ObjectInitializer);	
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	bool CheckCanAttack(AVRisingEnemyCharacter* OwnerAi);

	// 0 - Chase
	// 1 - Strafe
	// 2 - Attack
	
};
