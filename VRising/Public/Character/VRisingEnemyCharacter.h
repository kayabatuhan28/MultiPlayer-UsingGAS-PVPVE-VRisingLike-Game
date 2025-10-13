// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VRisingBaseCharacter.h"
#include "UI/Widget/EnemyOnHeadWidget.h"
#include "VRisingEnemyCharacter.generated.h"

class UBehaviorTree;
class AEnemyAiController;

UCLASS()
class VRISING_API AVRisingEnemyCharacter : public AVRisingBaseCharacter
{
	GENERATED_BODY()

public:
	AVRisingEnemyCharacter();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* WeaponMesh;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> EnemyOnHeadWidgetComp;

	UPROPERTY(BlueprintReadOnly)
	UEnemyOnHeadWidget* EnemyOnHeadWidget;

	virtual void Die() override;

	UFUNCTION(Server,Reliable)
	void Die_Server();

	UFUNCTION(NetMulticast, Reliable)
	void Die_Multicast();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"), Category = "Patrol")
	TArray<AActor*> PatrolPathActor;

	UPROPERTY(BlueprintReadWrite, Category = "Patrol")
	int32 PatrolPathIndex = 0;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Combat")
	bool bIsCombatModeActivated = false;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	float AttackDistance = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	float SkillDistance = 0.f;

	// Her skill saldırısı veya light attacktan sonra ne kadar bekleyecegi.Skill coldown ile normal attack cooldown ard arda bitmesi edge case durumunda 
	// ard arda 2 stateye girmemesi icin
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	float AttackRate = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TArray<UAnimMontage* > M_LightAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TArray<float > LightAttackDistance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TArray<UAnimMontage* > M_SkillAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TArray<UAnimMontage* > M_RangeSkillAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TArray<UAnimMontage* > M_Phase2SkillAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TArray<UAnimMontage* > M_Phase2RangeSkillAttack;
	
	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	float StrafeDistance = 500.f;

	UPROPERTY(BlueprintReadWrite, Category= "Combat")
	int32 AttackIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category= "Combat")
	int32 RangeSkillIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category= "Combat")
	int32 CloseSkillIndex = 0;
	
	UPROPERTY(EditDefaultsOnly)
	FString EnemyName = FString();

	UPROPERTY(BlueprintReadWrite, Category = "Target")
	TArray<AActor*> PotentialTarget = {};

	UPROPERTY(BlueprintReadWrite, Category = "Target")
	AActor* FocusedTarget;

	UPROPERTY(BlueprintReadWrite, Category = "Phase")
	bool IsPhase2Activated = false;

	UFUNCTION(BlueprintImplementableEvent)
	void Phase2Activate();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAttackRateCooldown();

	// Paladin Phase2 ye gectigi sirada abp de yerde stun statesine gecmesi icin 
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Phase2State")
	bool bIsAbpSetPhase2 = false;
	
	void CheckLightAttackCooldown(const FGameplayTag CallbackTag, int32 NewCount);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EnemyDefaultAttribute;
	
	UFUNCTION(BlueprintImplementableEvent)
	void DestroyBody();

	UFUNCTION(BlueprintCallable)
	void BindEnemyDelegates();
	
	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEnemyAiController> EnemyAIController;
	
};
