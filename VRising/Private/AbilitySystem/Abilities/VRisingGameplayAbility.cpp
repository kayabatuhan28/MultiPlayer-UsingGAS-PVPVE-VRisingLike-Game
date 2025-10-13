// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/VRisingGameplayAbility.h"

#include "Game/VRisingPlayerController.h"
#include "Kismet/GameplayStatics.h"

FRotator UVRisingGameplayAbility::CalculateInputDirection()
{
	FRotator Rot = FRotator();
	
	if (AVRisingPlayerController* PlayerController = Cast<AVRisingPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
	{
		float RightLeftValue = PlayerController->ActionValueX;
		float ForwardBackwardValue = PlayerController->ActionValueY;

		if (RightLeftValue == -1) // Press A
		{
			if (ForwardBackwardValue == 1) // Press A + W
			{
				Rot.Roll = 0.f;
				Rot.Pitch = 0.f;
				Rot.Yaw = 315.f;
				return Rot;
			}
			if (ForwardBackwardValue == -1) // Press A + S
			{
				Rot.Roll = 0.f;
				Rot.Pitch = 0.f;
				Rot.Yaw = 225.f;
				return Rot;
			}

			// Press Only A
			Rot.Roll = 0.f;
			Rot.Pitch = 0.f;
			Rot.Yaw = 270.f;
			return Rot;
			
		}


		if (RightLeftValue == 1) // Press D
		{
			if (ForwardBackwardValue == 1) // Press D + W
			{
				Rot.Roll = 0.f;
				Rot.Pitch = 0.f;
				Rot.Yaw = 45.f;
				return Rot;
			}
			
			if (ForwardBackwardValue == -1) // Press D + S
			{
				Rot.Roll = 0.f;
				Rot.Pitch = 0.f;
				Rot.Yaw = 135.f;
				return Rot;
			}

			// Press Only D
			Rot.Roll = 0.f;
			Rot.Pitch = 0.f;
			Rot.Yaw = 90.f;
			return Rot;
		}

		
		if (ForwardBackwardValue == -1) // Press S
		{
			if ( RightLeftValue == 1) // Press S + D
			{
				Rot.Roll = 0.f;
				Rot.Pitch = 0.f;
				Rot.Yaw = 135.f;
				return Rot;
			}

			if (RightLeftValue == -1) // Press S + A
			{
				Rot.Roll = 0.f;
				Rot.Pitch = 0.f;
				Rot.Yaw = 225.f;
				return Rot;
			}

			// Press Only S
			Rot.Roll = 0.f;
			Rot.Pitch = 0.f;
			Rot.Yaw = 180.f;
			return Rot;
		}


		if (ForwardBackwardValue == 1) // Press W
		{
			if (RightLeftValue == 1) // Press W + D
			{
				Rot.Roll = 0.f;
				Rot.Pitch = 0.f;
				Rot.Yaw = 45.f;
				return Rot;
			}

			if (RightLeftValue == -1) // Press W + A
			{
				Rot.Roll = 0.f;
				Rot.Pitch = 0.f;
				Rot.Yaw = 315.f;
				return Rot;
			}

			// Press Only W
			Rot.Roll = 0.f;
			Rot.Pitch = 0.f;
			Rot.Yaw = 0.f;
			return Rot;
		}

		
		// No Input Pressed
		Rot.Roll = 0.f;
		Rot.Pitch = 0.f;
		Rot.Yaw = 0.f;
		return Rot;
	}

	// Player Controller cast failed
	return Rot;
	
}
