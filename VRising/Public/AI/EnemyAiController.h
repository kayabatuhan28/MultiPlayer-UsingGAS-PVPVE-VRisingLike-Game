// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyAiController.generated.h"


UCLASS()
class VRISING_API AEnemyAiController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAiController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
};
