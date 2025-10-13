// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/VRisingGameMode.h"
#include "Game/VRisingPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AVRisingGameMode::AVRisingGameMode()
{
	PlayerControllerClass = AVRisingPlayerController::StaticClass();
	
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}