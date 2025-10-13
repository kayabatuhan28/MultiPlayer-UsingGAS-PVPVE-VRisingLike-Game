// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAiController.h"

#include "BehaviorTree/BlackboardComponent.h"

AEnemyAiController::AEnemyAiController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviourTreeComponent");
	check(BehaviorTreeComponent);

}
