// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSkillActor.generated.h"

UCLASS()
class VRISING_API ABaseSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseSkillActor();

	UFUNCTION(BlueprintCallable)
	bool CanHitDamageAbility(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	bool CanHitBuffAbility(AActor* OtherActor);

	// Projectileleri duvar engellemekte ama ok yagmurunu engellememesi adina eklendi.
	UPROPERTY(EditDefaultsOnly)
	bool bIsDestroyHitBlockProjectile = true;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
