// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleBumperPlayer.h"
#include "GameFramework/PlayerController.h"
#include "BattleBumperPlayerController.generated.h"

/**
 * 
 */


UCLASS()
class BATTLEBUMPER_API ABattleBumperPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Server, Unreliable, WithValidation)
		void Server_ReliableFunctionCallThatRunsOnServer(ABattleBumperPlayer* a, FVector NewLocation, FRotator NewRotation, float Velocity, float d, bool handbrake, bool shield);

	ABattleBumperPlayer* pawn;

	int inGameID;
	
};
