// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLevelScriptActor.h"
#include "TimerManager.h"
#include "Button.h"
#include "SessionButton.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"

void AMyLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	readyToHost = false;
	sessionList = new TArray<FSessionInfo *>();
	GetWorld()->GetTimerManager().SetTimer(serverListTimerHandler,
		this, &AMyLevelScriptActor::OnUpdateServerList, 2, true);
	if (MatchmakingWidgetClass)
	{
		MatchmakingWidget = CreateWidget<UUserWidget>(GetWorld(), 
			                                          MatchmakingWidgetClass);
		MatchmakingWidget->AddToViewport();
		UButton *connectButton = Cast<UButton>(MatchmakingWidget->
			GetWidgetFromName(TEXT("ConnectButton")));
		if (connectButton)
		{
			connectButton->OnClicked.AddDynamic(this, 
				&AMyLevelScriptActor::OnConnectClicked);
		}
		UButton *hostButton = Cast<UButton>(MatchmakingWidget->
			GetWidgetFromName(TEXT("HostButton")));
		if (hostButton)
		{
			hostButton->SetIsEnabled(false);
			hostButton->OnClicked.AddDynamic(this,
				&AMyLevelScriptActor::OnHostClicked);
		}
		sessionListScrollBox = Cast<UScrollBox>(MatchmakingWidget->
			GetWidgetFromName(TEXT("MyScrollBox")));
		APlayerController *pController = GetWorld()->GetFirstPlayerController();
		if (pController)
		{
			pController->bShowMouseCursor = true;
			pController->bEnableClickEvents = true;
			pController->bEnableMouseOverEvents = true;
		}
	}
}

void AMyLevelScriptActor::OnUpdateServerList()
{
	if (tcpClient)
	{
		if (tcpClient->IsConnected())
		{
			UButton *connectButton = Cast<UButton>(MatchmakingWidget->
				GetWidgetFromName(TEXT("ConnectButton")));
			if (connectButton)
			{
				connectButton->SetIsEnabled(false);
			}
			UButton *hostButton = Cast<UButton>(MatchmakingWidget->
				GetWidgetFromName(TEXT("HostButton")));
			if (hostButton)
			{
				hostButton->SetIsEnabled(true);
			}
			tcpClient->RequestSessionList();
			if (readyToHost)
			{
				APlayerController* pController = GetWorld()->GetFirstPlayerController();
				if (pController)
				{
					FString comand = "open 40.113.114.149:" + FString::FromInt(hostPort);
					pController->ConsoleCommand(comand);
				}
			}
		}
		if (sessionList->Num() > 0)
		{
			if ((MatchmakingWidget) && (sessionListScrollBox))
			{
				TArray<UWidget*> allChildren = sessionListScrollBox->GetAllChildren();
				for (int i = 0; i < allChildren.Num(); i++)
				{
					allChildren[i]->RemoveFromParent();
					UE_LOG(LogTemp, Log, TEXT("CLEARRRRR!"));
				}
				for (int i = 0; i < sessionList->Num(); i++)
				{
					UVerticalBox *itemWidgetBox = NewObject<UVerticalBox>();
					sessionListScrollBox->AddChild(itemWidgetBox);
					USessionButton *itemWidget = NewObject<USessionButton>(this);
					
					itemWidget->SetSessionInfo((*sessionList)[i]);
					UTextBlock *itemWidgetText = NewObject<UTextBlock>();
					itemWidgetText->SetText(FText::FromString((*sessionList)[i]->name));
					itemWidget->AddChild(itemWidgetText);
					UVerticalBoxSlot *slot = itemWidgetBox->AddChildToVerticalBox(itemWidget);
					static FMargin Padding(5);
					slot->SetPadding(Padding);
				}
			}
		}
	}
}

void AMyLevelScriptActor::OnConnectClicked()
{
	tcpClient = new TCPClient(this);
}

void AMyLevelScriptActor::OnHostClicked()
{
	tcpClient->HostNewGame("My Session", "7777");
	hosting = true;
}

void AMyLevelScriptActor::UpdateSessionList(FString serverinfo)
{

		TArray<FString> Out;
		serverinfo.ParseIntoArray(Out, TEXT("|"), true);
		if (Out.Num() > 3) {
			for (int i = 1; i < Out.Num() - 3; i += 4)
			{
				FSessionInfo* sInfo = new FSessionInfo();
				sInfo->id = FCString::Atoi(*Out[i]);
				sInfo->name = Out[i + 1];
				sInfo->serverip = Out[i + 2];
				sInfo->serverport = FCString::Atoi(*Out[i + 3]);
				bool sExist = false;
				for (int j = 0; j < sessionList->Num(); j++)
				{
					if ((*sessionList)[j]->id == sInfo->id) {
						sExist = true;
					}
				}
				if (!sExist) {
					sessionList->Add(sInfo);
				}
			}
		}
}

void AMyLevelScriptActor::StartGameHost(int port)
{
	hostPort = port;
	readyToHost = true;
}
