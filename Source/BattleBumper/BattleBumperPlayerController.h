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

	ABattleBumperPlayer* pawn;
	
};
