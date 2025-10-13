// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/VRisingCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/VRisingAbilitySystemComponent.h"
#include "AbilitySystem/VRisingAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/VRisingPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Game/VRisingPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/HUD/VRisingHUD.h"
#include "UI/Widget/PlayerOnHeadWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"


AVRisingCharacter::AVRisingCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); 
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; 
	
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PlayerOnHeadWidgetComp = CreateDefaultSubobject<UWidgetComponent>("OnHeadWidget");
	PlayerOnHeadWidgetComp->SetupAttachment(GetRootComponent());

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), "Socket_Staff");
	WeaponMesh->SetCollisionProfileName("NoCollision");
}

void AVRisingCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVRisingCharacter, SelectedWeapon);
	DOREPLIFETIME(AVRisingCharacter, ReplicatedCursorHitLocation);
}

void AVRisingCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
	
}

void AVRisingCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void AVRisingCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AVRisingCharacter::Die()
{
	Super::Die();

	TestDie();
	
	Die_Server();
}

void AVRisingCharacter::Die_Server_Implementation()
{
	Die_Multicast();
}

void AVRisingCharacter::Die_Multicast_Implementation()
{
	if (PlayerOnHeadWidget)
	{
		PlayerOnHeadWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (AVRisingPlayerController* AuraPlayerController = Cast<AVRisingPlayerController>(GetController()))
	{
		if (AVRisingHUD* AuraHUD = Cast<AVRisingHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->OverlayWidgetController->OnPlayerDie.Broadcast();
		}
	}
}

// Hit Mouse Cursor Replication
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
void AVRisingCharacter::SetReplicatedCursorLocation()
{
	APlayerController* HoldController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (HoldController)
	{
		FHitResult HitResult;
		HoldController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility),true,HitResult);
		SetCursorLocation_Server(HitResult.Location);
	}
}

void AVRisingCharacter::SetCursorLocation_Server_Implementation(FVector NewLoc)
{
	SetCursorLocation_Multicast(NewLoc);
}

void AVRisingCharacter::SetCursorLocation_Multicast_Implementation(FVector NewLoc)
{
	ReplicatedCursorHitLocation = NewLoc;
}
// Hit Mouse Cursor Replication
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

void AVRisingCharacter::PlayerStunned(const FGameplayTag CallbackTag, int32 NewCount)
{
	
	if (PlayerOnHeadWidget && NewCount > 0)
	{
		PlayerOnHeadWidget->OnPlayerStunned.Broadcast(true);
	}
	else
	{
		PlayerOnHeadWidget->OnPlayerStunned.Broadcast(false);
	}
}

void AVRisingCharacter::PlayerSlowed(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (PlayerOnHeadWidget && NewCount > 0)
	{
		PlayerOnHeadWidget->OnPlayerSlowed.Broadcast(true);
	}
	else
	{
		PlayerOnHeadWidget->OnPlayerSlowed.Broadcast(false);
	}
}

void AVRisingCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerOnHeadWidget = Cast<UPlayerOnHeadWidget>(PlayerOnHeadWidgetComp->GetUserWidgetObject());
	
}


void AVRisingCharacter::BindAttributeDelegateforPlayer()
{
	if (!IsValid(PlayerOnHeadWidget)) return;
	
	if (const UVRisingAttributeSet* VRisingAS = Cast<UVRisingAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VRisingAS->GetHealthAttribute()).AddWeakLambda(this,
			[this](const FOnAttributeChangeData& Data)
			{
				if (IsValid(PlayerOnHeadWidget))
				{
					PlayerOnHeadWidget->OnPlayerHealthChanged.Broadcast(Data.NewValue);
				}
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VRisingAS->GetMaxHealthAttribute()).AddWeakLambda(this, 
			[this](const FOnAttributeChangeData& Data)
			{
				if (IsValid(PlayerOnHeadWidget))
				{
					PlayerOnHeadWidget->OnPlayerMaxHealthChanged.Broadcast(Data.NewValue);
				}
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VRisingAS->GetManaAttribute()).AddWeakLambda(this, 
			[this](const FOnAttributeChangeData& Data)
			{
				if (IsValid(PlayerOnHeadWidget))
				{
					PlayerOnHeadWidget->OnPlayerManaChanged.Broadcast(Data.NewValue);
				}
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VRisingAS->GetMaxManaAttribute()).AddWeakLambda(this, 
			[this](const FOnAttributeChangeData& Data)
			{
				if (IsValid(PlayerOnHeadWidget))
				{
					PlayerOnHeadWidget->OnPlayerMaxManaChanged.Broadcast(Data.NewValue);
				}
			}
		);


		// Initialize player start attribute
		PlayerOnHeadWidget->OnPlayerMaxHealthChanged.Broadcast(VRisingAS->GetMaxHealth());
		PlayerOnHeadWidget->OnPlayerHealthChanged.Broadcast(VRisingAS->GetHealth());
		PlayerOnHeadWidget->OnPlayerMaxManaChanged.Broadcast(VRisingAS->GetMaxMana());
		PlayerOnHeadWidget->OnPlayerManaChanged.Broadcast(VRisingAS->GetMana());

		
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("AbilitySystem.Abilities.Stun"), EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AVRisingCharacter::PlayerStunned
	);

	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("AbilitySystem.Abilities.DamagedStun"), EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AVRisingCharacter::PlayerStunned
	);

	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("AbilitySystem.Effects.Slowed"), EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AVRisingCharacter::PlayerSlowed
	);
}

void AVRisingCharacter::WeaponChanged_Server_Implementation(EWeaponType NewWeaponType)
{
	if (UVRisingAbilitySystemComponent* VASC = Cast<UVRisingAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		if (NewWeaponType == EWeaponType::Staff)
		{
			SelectedWeapon = EWeaponType::Staff;
		}
		else if (NewWeaponType == EWeaponType::Bow)
		{
			SelectedWeapon = EWeaponType::Bow;
		}
		else if (NewWeaponType == EWeaponType::GreatAxe)
		{
			SelectedWeapon = EWeaponType::GreatAxe;
		}
		
		WeaponSwap(SelectedWeapon);
	}
}

void AVRisingCharacter::WeaponChanged(EWeaponType NewWeaponType)
{
	WeaponChanged_Server(NewWeaponType);

	if (AVRisingPlayerController* AuraPlayerController = Cast<AVRisingPlayerController>(GetController()))
	{
		if (AVRisingHUD* AuraHUD = Cast<AVRisingHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->OverlayWidgetController->OnWeaponChanged.Broadcast(NewWeaponType, WeaponChangeInfo);
		}
	}
}



void AVRisingCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AVRisingPlayerState* VRisingPlayerState = GetPlayerState<AVRisingPlayerState>();
	check(VRisingPlayerState);
	
	VRisingPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(VRisingPlayerState, this);
	Cast<UVRisingAbilitySystemComponent>(VRisingPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	AbilitySystemComponent = VRisingPlayerState->GetAbilitySystemComponent();
	AttributeSet = VRisingPlayerState->GetAttributeSet();

	
	if (AVRisingPlayerController* AuraPlayerController = Cast<AVRisingPlayerController>(GetController()))
	{
		if (AVRisingHUD* AuraHUD = Cast<AVRisingHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InıtOverlay(AuraPlayerController, VRisingPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	

	// Attributeler OnRep replike oldugu icin serverde setledigimiz zaman, clientlerdede otomatik setlenir o yüzden buraya koymamiz yeterli baslangic
	// icin Baslangic icin kendimize gameplayEffect uygulayip default attributeleri setleriz
	InitializeDefaultAttributes();
}
