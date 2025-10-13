// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VRisingBaseCharacter.h"
#include "AbilitySystem/Data/WeaponChangeInfo.h"
#include "VRisingCharacter.generated.h"

class UPlayerOverlayWidget;
class UPlayerOnHeadWidget;



UCLASS(Blueprintable)
class AVRisingCharacter : public AVRisingBaseCharacter
{
	GENERATED_BODY()

public:
	AVRisingCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> PlayerOnHeadWidgetComp;

	UPROPERTY(BlueprintReadOnly)
	UPlayerOnHeadWidget* PlayerOnHeadWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Montage")
	TArray<UAnimMontage*> M_BasicAttack;

	UPROPERTY(BlueprintReadWrite, Category = "Attack|Data")
	int32 BasicComboCount = 0;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data")
	TObjectPtr<UWeaponChangeInfo> WeaponChangeInfo;

	UFUNCTION(Server,Unreliable)
	void WeaponChanged_Server(EWeaponType NewWeaponType);

	UFUNCTION(BlueprintCallable)
	void WeaponChanged(EWeaponType NewWeaponType);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	EWeaponType SelectedWeapon = EWeaponType::Staff;

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponSwap(EWeaponType NewWeapon);
	
	virtual void Die() override;

	UFUNCTION(Server,Reliable)
	void Die_Server();

	UFUNCTION(NetMulticast, Reliable)
	void Die_Multicast();

	UFUNCTION(BlueprintImplementableEvent)
	void TestDie();

	// Replicated Cursor Hit Location
	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector ReplicatedCursorHitLocation = FVector();

	UFUNCTION(BlueprintCallable)
	void SetReplicatedCursorLocation();

	UFUNCTION(Server,Reliable)
	void SetCursorLocation_Server(FVector NewLoc);

	UFUNCTION(NetMulticast, Reliable)
	void SetCursorLocation_Multicast(FVector NewLoc);
	
	void PlayerStunned(const FGameplayTag CallbackTag, int32 NewCount);

	void PlayerSlowed(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION(BlueprintCallable)
	void BindAttributeDelegateforPlayer();

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	void InitAbilityActorInfo() override;
};

