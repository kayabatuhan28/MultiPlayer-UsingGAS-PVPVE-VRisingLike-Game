// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/VRisingBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/VRisingAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "UI/Widget/DamageTextComponent.h"


AVRisingBaseCharacter::AVRisingBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
}



void AVRisingBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVRisingBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRisingBaseCharacter::InitAbilityActorInfo()
{
}

void AVRisingBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent()); // Target yerine selfide kullanabilirdik.
}

void AVRisingBaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AVRisingBaseCharacter::AddCharacterAbilities()
{
	UVRisingAbilitySystemComponent* VRisingASC = CastChecked<UVRisingAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	VRisingASC->AddCharacterAbilities(StartupAbilities);
	VRisingASC->AddCharacterPassiveAbilities(StartupPassiveAbilities); 
}

void AVRisingBaseCharacter::ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bIsHealing)
{
	ShowDamageNumber_Multicast(DamageAmount, TargetCharacter, bIsHealing);
}

void AVRisingBaseCharacter::ShowDamageNumber_Multicast_Implementation(float DamageAmount, ACharacter* TargetCharacter,
	bool bIsHealing)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		
		
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		// Widget animasyonu vurdugumuz yerde kalması icin hemen attachledikten sonra detachledik
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		DamageText->SetDamageText(DamageAmount, bIsHealing);
	}
}

UAbilitySystemComponent* AVRisingBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AVRisingBaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* AVRisingBaseCharacter::GetAvatar_Implementation()
{
	return this;
}

void AVRisingBaseCharacter::Die()
{
	MulticastHandleDeath();
}

void AVRisingBaseCharacter::MulticastHandleDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // Duvar, floor vs gibi seylere response verebilmesi icin
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bDead = true;
}



