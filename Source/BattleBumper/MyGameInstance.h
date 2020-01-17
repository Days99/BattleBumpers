// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleBumperPlayer.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"


const static FName SESSION_NAME = TEXT("MySession");

/**
 * 
 */
UCLASS()
class BATTLEBUMPER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

//private:
//	virtual void Init();
//
public:
//	UFUNCTION(BlueprintCallable)
//	void LoadMenu();
//
//	UPROPERTY(EditDefaultsOnly, Category = "UI")
//	TSubclassOf<class UUserWidget> MenuWidgetClass;
//	UUserWidget* MenuWidget;
//
//	IOnlineSessionPtr SessionInterface;
//	TSharedRef<class FOnlineSessionSearch> SessionSearch;
//
//	UFUNCTION(BlueprintCallable)
//	void Host();
//
//	UFUNCTION(BlueprintCallable)
//	void Join();
//
//
//	void CreateSession();
//	void DestroySession();
//	void StartSession();
//	void SearchSession();
//	void JoinSession();
//
//	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
//	void OnFindSessionsComplete(bool bWasSuccessful);
//	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
//	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
//	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type type);

	int GenerateID(ABattleBumperPlayer* p);

	UPROPERTY(Replicated)
	int maxPlayers;

	void AddPlayer(ABattleBumperPlayer* p);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMaxPlayers(int max);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	int GetMaxPlayers();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetGame();



	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ClearPlayers();

	UFUNCTION(NetMulticast, Reliable)
	void RemovePlayer(ABattleBumperPlayer * p);

	void StartGame();


	ABattleBumperPlayer* GetRandomPlayer();



	TArray<ABattleBumperPlayer*> players;
	
};
