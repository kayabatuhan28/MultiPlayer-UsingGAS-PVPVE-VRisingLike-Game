// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRisingGameMode.generated.h"

class UCharacterClassInfo;

UCLASS(minimalapi)
class AVRisingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AVRisingGameMode();
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};



