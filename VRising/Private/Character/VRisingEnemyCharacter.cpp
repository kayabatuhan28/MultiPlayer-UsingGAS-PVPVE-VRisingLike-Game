// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VRisingEnemyCharacter.h"
#include "AbilitySystem/VRisingAbilitySystemComponent.h"
#include "AbilitySystem/VRisingAbilitySystemLibrary.h"
#include "AbilitySystem/VRisingAttributeSet.h"
#include "AI/EnemyAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widget/EnemyOnHeadWidget.h"

AVRisingEnemyCharacter::AVRisingEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UVRisingAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UVRisingAttributeSet>("AttributeSet");

	EnemyOnHeadWidgetComp = CreateDefaultSubobject<UWidgetComponent>("OnHeadWidget");
	EnemyOnHeadWidgetComp->SetupAttachment(GetRootComponent());
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), "Socket_GreatAxe");
	WeaponMesh->SetCollisionProfileName("NoCollision");
}

void AVRisingEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority()) return;

	EnemyAIController = Cast<AEnemyAiController>(NewController);
	if (EnemyAIController && BehaviorTree)
	{
		EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		EnemyAIController->RunBehaviorTree(BehaviorTree);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("EnemyAiController cast fail or behaviour tree not selected"));
	}

	
}

void AVRisingEnemyCharacter::Die()
{
	Super::Die();
	
	Die_Server();
}

void AVRisingEnemyCharacter::Die_Server_Implementation()
{
	Die_Multicast();
}

void AVRisingEnemyCharacter::Die_Multicast_Implementation()
{
	bDead = true;
	if (EnemyOnHeadWidget)
	{
		EnemyOnHeadWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	DestroyBody();
}

void AVRisingEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVRisingEnemyCharacter, bIsCombatModeActivated);
	DOREPLIFETIME(AVRisingEnemyCharacter, bIsAbpSetPhase2);
}

void AVRisingEnemyCharacter::CheckLightAttackCooldown(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		if (EnemyAIController)
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsLightAttackCd", true);
		}
	}
	else
	{
		if (EnemyAIController)
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsLightAttackCd", false);
		}
	}
}

void AVRisingEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	EnemyOnHeadWidget = Cast<UEnemyOnHeadWidget>(EnemyOnHeadWidgetComp->GetUserWidgetObject());
	
}

void AVRisingEnemyCharacter::BindEnemyDelegates()
{
	if (!IsValid(EnemyOnHeadWidget)) return;

	if (const UVRisingAttributeSet* VRisingAS = Cast<UVRisingAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VRisingAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				EnemyOnHeadWidget->OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VRisingAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				EnemyOnHeadWidget->OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		// Initialize start attribute
		EnemyOnHeadWidget->OnMaxHealthChanged.Broadcast(VRisingAS->GetMaxHealth());
		EnemyOnHeadWidget->OnHealthChanged.Broadcast(VRisingAS->GetHealth());
		EnemyOnHeadWidget->SetEnemyName(EnemyName);
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("AbilitySystem.Cooldown.EnemyLightAttack"), EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AVRisingEnemyCharacter::CheckLightAttackCooldown
	);

}

void AVRisingEnemyCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UVRisingAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AVRisingEnemyCharacter::InitializeDefaultAttributes() const
{
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EnemyDefaultAttribute, Level, VitalAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());

	//UVRisingAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
