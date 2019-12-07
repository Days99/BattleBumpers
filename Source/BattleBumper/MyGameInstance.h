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

	UPROPERTY(Replicated)
	int maxPlayers;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMaxPlayers(int max);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	int GetMaxPlayers();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ClearPlayers();

	void RemovePlayer(ABattleBumperPlayer * p);

	void StartGame();


	ABattleBumperPlayer* GetRandomPlayer();



	TArray<ABattleBumperPlayer*> players;
	
};
