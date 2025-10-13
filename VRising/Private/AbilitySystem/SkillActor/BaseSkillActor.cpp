// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SkillActor/BaseSkillActor.h"

ABaseSkillActor::ABaseSkillActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

bool ABaseSkillActor::CanHitDamageAbility(AActor* OtherActor)
{
	if (OtherActor->Tags.Contains("BlockProjectile") && bIsDestroyHitBlockProjectile == true)
	{
		K2_DestroyActor();
		return false;
	}

	if (GetInstigator() == OtherActor)
	{
		return false;
	}

	if (OtherActor->Tags.Contains("Enemy") || OtherActor->Tags.Contains("PvpEnabled"))
	{
		return true;
	}

	return false;
}

bool ABaseSkillActor::CanHitBuffAbility(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Enemy") || (OtherActor->ActorHasTag("PvpEnabled") && OtherActor != GetInstigator()))
	{
		return false;
	}

	if (OtherActor == GetInstigator() || OtherActor->ActorHasTag("Player"))
	{
		return true;
	}

	return false;
}

void ABaseSkillActor::BeginPlay()
{
	Super::BeginPlay();

	
}

void ABaseSkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

