// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SkillActor/EnemySkillBase.h"


AEnemySkillBase::AEnemySkillBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}


void AEnemySkillBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AEnemySkillBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AEnemySkillBase::CanTakeHit(AActor* OtherActor)
{
	if (bIsBuffSkill == true)
	{
		if (OtherActor->ActorHasTag("Enemy"))
		{
			return true;
		}
	}

	if (bIsBuffSkill == false)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			return true;
		}
	}
	
	return false;
}

