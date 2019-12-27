// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuLevelScript.h"
#include "TimerManager.h"
#include "SessionButton.h"
#include "Button.h"
#include <Runtime\UMG\Public\Components\VerticalBox.h>
#include <Runtime\UMG\Public\Components\VerticalBoxSlot.h>
#include <Runtime\UMG\Public\Components\TextBlock.h>

void AMenuLevelScript::BeginPlay()
{
	Super::BeginPlay();

	readyToHost = false;
	readyToUpdateList = false;

	GetWorld()->GetTimerManager().SetTimer(serverListTimerHandler, this, &AMenuLevelScript::OnUpdateServerList, 2, true);
	sessionList = new TArray<FSessionInfo*>();
	if (MatchmakingWidgetClass)
	{
		MatchmakingWidget = CreateWidget<UUserWidget>(GetWorld(), MatchmakingWidgetClass);
		MatchmakingWidget->AddToViewport();
		UButton* connectButton = Cast<UButton>(MatchmakingWidget->GetWidgetFromName(TEXT("ConnectButton")));
		if (connectButton)
		{
			connectButton->OnClicked.AddDynamic(this, &AMenuLevelScript::OnConnectClicked);
		}
		UButton* hostButton = Cast<UButton>(MatchmakingWidget->GetWidgetFromName(TEXT("HostButton")));
		if (hostButton)
		{
			hostButton->SetIsEnabled(false);
			hostButton->OnClicked.AddDynamic(this, &AMenuLevelScript::OnHostClicked);
		}
		sessionListScrollBox = Cast<UScrollBox>(MatchmakingWidget->GetWidgetFromName(TEXT("MyScrollBox")));
		APlayerController* pController = GetWorld()->GetFirstPlayerController();
		if (pController)
		{
			pController->bShowMouseCursor = true;
			pController->bEnableClickEvents = true;
			pController->bEnableMouseOverEvents = true;
		}
	}
}

void AMenuLevelScript::OnUpdateServerList()
{
	if (TCPclienty) {
		if (TCPclienty->IsConnected()) {
			UButton* connectButton = Cast<UButton>(MatchmakingWidget->GetWidgetFromName(TEXT("ConnectButton")));
			if (connectButton) {
				connectButton->SetIsEnabled(false);
			}
			UButton* hostButton = Cast<UButton>(MatchmakingWidget->GetWidgetFromName(TEXT("HostButton")));
			if (hostButton)
			{
				hostButton->SetIsEnabled(true);
			}
		}

		if (sessionList->Num() > 0) {
			if ((MatchmakingWidget) && (sessionListScrollBox)) {
				TArray<UWidget*> allChildren = sessionListScrollBox->GetAllChildren();

				for (int i = 0; i < allChildren.Num(); i++) {
					allChildren[i]->RemoveFromParent();
				}
				for (int i = 0; i < sessionList->Num(); i++) {
					UVerticalBox* itemWidgetBox = NewObject<UVerticalBox>();
					sessionListScrollBox->AddChild(itemWidgetBox);
					USessionButton* itemButton = NewObject<USessionButton>();

					itemButton->SetSessionInfo((*sessionList)[i]);

					UTextBlock* itemText = NewObject<UTextBlock>();
					itemText->SetText(FText::FromString((*sessionList)[i]->name));
					itemButton->AddChild(itemText);
					UVerticalBoxSlot* slot = itemWidgetBox->AddChildToVerticalBox(itemButton);
					static FMargin Padding(5);
					slot->SetPadding(Padding);
				}

			}
		}
	}
}

void AMenuLevelScript::OnConnectClicked()
{
	TCPclienty = new TCPClient(this);
}
void AMenuLevelScript::OnHostClicked()
{
}

void AMenuLevelScript::UpdateSessionList(FString serverinfo)
{
	TArray<FString> Out;
	serverinfo.ParseIntoArray(Out, TEXT("|"), true);

	for (int i = 1; i < Out.Num() - 3; i += 4) {
		FSessionInfo* tempInfo = new FSessionInfo;
		tempInfo->id = FCString::Atoi(*Out[i]);
		tempInfo->name = Out[i + 1];
		tempInfo->serverip = Out[i + 2];
		tempInfo->serverport = FCString::Atoi(*Out[i + 3]);

		sessionList->Add(tempInfo);
	}
}

void AMenuLevelScript::StartGameHost(int port)
{
	hostPort = port;
	readyToHost = true;
}
