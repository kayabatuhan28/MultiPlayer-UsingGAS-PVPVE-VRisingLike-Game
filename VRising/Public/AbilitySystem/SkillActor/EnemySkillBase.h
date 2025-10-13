// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySkillBase.generated.h"

UCLASS()
class VRISING_API AEnemySkillBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySkillBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	bool bIsDestroyHitBlockProjectile = true;

	UPROPERTY(EditDefaultsOnly)
	bool bIsBuffSkill = false;

	UFUNCTION(BlueprintCallable)
	bool CanTakeHit(AActor* OtherActor);

};
