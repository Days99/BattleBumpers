// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Components/ScrollBox.h"
#include "TCPClient.h"
#include "MenuLevelScript.generated.h"

USTRUCT()
struct FSessionInfo {
	GENERATED_BODY()

		UPROPERTY()
		int id;

	UPROPERTY()
		FString name;

	UPROPERTY()
		FString serverip;

	UPROPERTY()
		int serverport;
};

UCLASS()
class BATTLEBUMPER_API AMenuLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	TCPClient* TCPclienty;

	TArray<FSessionInfo*>* sessionList;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> MatchmakingWidgetClass;
	UUserWidget* MatchmakingWidget;
	UScrollBox* sessionListScrollBox;
	FTimerHandle serverListTimerHandler;

	bool readyToHost;
	int hostPort;
	bool readyToUpdateList;

	UFUNCTION()
		void OnUpdateServerList();

	UFUNCTION()
		void OnConnectClicked();
	UFUNCTION()
		void OnHostClicked();

public:
	void UpdateSessionList(FString serverinfo);
	void StartGameHost(int port);
};
