// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BattleBumperPlayer.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBUMPER_API AMyPlayerState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float ReturnPlayerVelocity(ABattleBumperPlayer * p);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		float ReturnPlayerDamage(ABattleBumperPlayer* p);
};
