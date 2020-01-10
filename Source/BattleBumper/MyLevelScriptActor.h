// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Components/ScrollBox.h"
#include "TCPClient.h"
#include "MyLevelScriptActor.generated.h"


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
class BATTLEBUMPER_API AMyLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	
	TArray<FSessionInfo*> *sessionList;

	TCPClient *tcpClient;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MatchmakingWidgetClass;
	UUserWidget *MatchmakingWidget;
	UScrollBox *sessionListScrollBox;
	FTimerHandle serverListTimerHandler;

	UFUNCTION()
	void OnUpdateServerList();
	UFUNCTION()
	void OnConnectClicked();
	UFUNCTION()
	void OnHostClicked();

	bool hosting;
	bool readyToHost;
	int hostPort;

public:
	void UpdateSessionList(FString serverinfo);
	void StartGameHost(int port);
	
};
