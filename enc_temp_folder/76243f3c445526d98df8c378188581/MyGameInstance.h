// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleBumperPlayer.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBUMPER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	int GenerateID(ABattleBumperPlayer* p);

	TArray<ABattleBumperPlayer*> players;
	
};
