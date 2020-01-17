// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLevelScriptActor.h"
#include "TimerManager.h"
#include "Button.h"
#include "SessionButton.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "BattleBumperPlayerController.h"


void AMyLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	readyToHost = false;
	sessionList = new TArray<FSessionInfo *>();
	GetWorld()->GetTimerManager().SetTimer(serverListTimerHandler,
		this, &AMyLevelScriptActor::OnUpdateServerList,0.5f, true);
	if (MatchmakingWidgetClass)
	{
		MatchmakingWidget = CreateWidget<UUserWidget>(GetWorld(), 
			                                          MatchmakingWidgetClass);
		MatchmakingWidget->AddToViewport();
		UButton *connectButton = Cast<UButton>(MatchmakingWidget->
			GetWidgetFromName(TEXT("ConnectButton")));

		UButton *TwoPlayerButton = Cast<UButton>(MatchmakingWidget->
			GetWidgetFromName(TEXT("2PButton")));
		UButton *FourPlayerButton = Cast<UButton>(MatchmakingWidget->
			GetWidgetFromName(TEXT("4PButton")));
		UButton *EightPlayerButton = Cast<UButton>(MatchmakingWidget->
			GetWidgetFromName(TEXT("8PButton")));

		UCheckBox *checkBox1 = Cast<UCheckBox>(MatchmakingWidget->
			GetWidgetFromName(TEXT("1MapCheck")));
		UCheckBox *checkBox2 = Cast<UCheckBox>(MatchmakingWidget->
			GetWidgetFromName(TEXT("2MapCheck")));
		UCheckBox* checkBox3 = Cast<UCheckBox>(MatchmakingWidget->
			GetWidgetFromName(TEXT("3MapCheck")));

		UTextBlock *LevelBox1 = Cast<UTextBlock>(MatchmakingWidget->
			GetWidgetFromName(TEXT("TextLevel0")));
		UTextBlock *LevelBox2 = Cast<UTextBlock>(MatchmakingWidget->
			GetWidgetFromName(TEXT("TextLevel1")));
		UTextBlock* LevelBox3 = Cast<UTextBlock>(MatchmakingWidget->
			GetWidgetFromName(TEXT("TextLevel2")));

		if (LevelBox1) {
			LevelBox1->SetVisibility(ESlateVisibility::Hidden);
		}
		if (LevelBox2) {
			LevelBox2->SetVisibility(ESlateVisibility::Hidden);
		}
		if (LevelBox3) {
			LevelBox3->SetVisibility(ESlateVisibility::Hidden);
		}


		if (checkBox1) {
			checkBox1->SetVisibility(ESlateVisibility::Hidden);
		}
		if (checkBox2) {
			checkBox2->SetVisibility(ESlateVisibility::Hidden);
		}
		if (checkBox3) {
			checkBox3->SetVisibility(ESlateVisibility::Hidden);
		}


		if (TwoPlayerButton) {
			TwoPlayerButton->SetVisibility(ESlateVisibility::Hidden);
			TwoPlayerButton->OnClicked.AddDynamic(this,
				&AMyLevelScriptActor::OnHostClicked2);
		}
		if (FourPlayerButton) {
			FourPlayerButton->SetVisibility(ESlateVisibility::Hidden);
			FourPlayerButton->OnClicked.AddDynamic(this,
				&AMyLevelScriptActor::OnHostClicked4);
		}
		if (EightPlayerButton) {
			EightPlayerButton->SetVisibility(ESlateVisibility::Hidden);
			EightPlayerButton->OnClicked.AddDynamic(this,
				&AMyLevelScriptActor::OnHostClicked8);
		}
		if (connectButton)
		{
			connectButton->OnClicked.AddDynamic(this, 
				&AMyLevelScriptActor::OnConnectClicked);
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
				connectButton->SetVisibility(ESlateVisibility::Hidden);
			}
			UCheckBox *checkBox1 = Cast<UCheckBox>(MatchmakingWidget->
				GetWidgetFromName(TEXT("1MapCheck")));
			UCheckBox *checkBox2 = Cast<UCheckBox>(MatchmakingWidget->
				GetWidgetFromName(TEXT("2MapCheck")));
			UCheckBox* checkBox3 = Cast<UCheckBox>(MatchmakingWidget->
				GetWidgetFromName(TEXT("3MapCheck")));

			UTextBlock *LevelBox1 = Cast<UTextBlock>(MatchmakingWidget->
				GetWidgetFromName(TEXT("TextLevel0")));
			UTextBlock *LevelBox2 = Cast<UTextBlock>(MatchmakingWidget->
				GetWidgetFromName(TEXT("TextLevel1")));
			UTextBlock* LevelBox3 = Cast<UTextBlock>(MatchmakingWidget->
				GetWidgetFromName(TEXT("TextLevel2")));

			if (LevelBox1) {
				LevelBox1->SetVisibility(ESlateVisibility::Visible);
			}
			if (LevelBox2) {
				LevelBox2->SetVisibility(ESlateVisibility::Visible);
			}
			if (LevelBox3) {
				LevelBox3->SetVisibility(ESlateVisibility::Visible);
			}


			if (checkBox1) {
				checkBox1->SetVisibility(ESlateVisibility::Visible);
			}
			if (checkBox2) {
				checkBox2->SetVisibility(ESlateVisibility::Visible);
			}
			if (checkBox3) {
				checkBox3->SetVisibility(ESlateVisibility::Visible);
			}


			if (checkBox1->IsChecked()) {
				checkBox2->SetCheckedState(ECheckBoxState::Unchecked);
				checkBox3->SetCheckedState(ECheckBoxState::Unchecked);
				Map = 0;
			}
			if (checkBox2->IsChecked()) {
				checkBox1->SetCheckedState(ECheckBoxState::Unchecked);
				checkBox3->SetCheckedState(ECheckBoxState::Unchecked);
				Map = 1;
			}
			if (checkBox3->IsChecked()) {
				checkBox1->SetCheckedState(ECheckBoxState::Unchecked);
				checkBox2->SetCheckedState(ECheckBoxState::Unchecked);
				Map = 2;
			}
			UButton *TwoPlayerButton = Cast<UButton>(MatchmakingWidget->
				GetWidgetFromName(TEXT("2PButton")));
			UButton *FourPlayerButton = Cast<UButton>(MatchmakingWidget->
				GetWidgetFromName(TEXT("4PButton")));
			UButton *EightPlayerButton = Cast<UButton>(MatchmakingWidget->
				GetWidgetFromName(TEXT("8PButton")));
			if (TwoPlayerButton) {
				TwoPlayerButton->SetVisibility(ESlateVisibility::Visible);
			}
			if (FourPlayerButton) {
				FourPlayerButton->SetVisibility(ESlateVisibility::Visible);
			}
			if (EightPlayerButton) {
				EightPlayerButton->SetVisibility(ESlateVisibility::Visible);
			}
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

void AMyLevelScriptActor::OnHostClicked2()
{
	ABattleBumperPlayerController* pController = Cast<ABattleBumperPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pController) {
		pController->maxNumbP = 2;
	}
	tcpClient->HostNewGame("My Session", "7777", FString::FromInt(2), FString::FromInt(Map));
	hosting = true;
}
void AMyLevelScriptActor::OnHostClicked4()
{
	ABattleBumperPlayerController* pController = Cast<ABattleBumperPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pController) {
		pController->maxNumbP = 4;
	}
	tcpClient->HostNewGame("My Session", "7777", FString::FromInt(4), FString::FromInt(Map));
	hosting = true;
}
void AMyLevelScriptActor::OnHostClicked8()
{
	ABattleBumperPlayerController* pController = Cast<ABattleBumperPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pController) {
		pController->maxNumbP = 8;
	}
	tcpClient->HostNewGame("My Session", "7777", FString::FromInt(8), FString::FromInt(Map));
	hosting = true;
}

void AMyLevelScriptActor::JoinSession(FString serverinfo)
{

		TArray<FString> Out;
		serverinfo.ParseIntoArray(Out, TEXT("|"), true);
		if (Out.Num() > 4) {
			int id = 0;
			int port = 0;
			for (int i = 1; i < Out.Num() - 4; i += 5)
			{
				FSessionInfo* sInfo = new FSessionInfo();
				sInfo->id = FCString::Atoi(*Out[i]);
				sInfo->name = Out[i + 1];
				sInfo->serverip = Out[i + 2];
				sInfo->serverport = FCString::Atoi(*Out[i + 3]);
				port = sInfo->serverport;
				id = FCString::Atoi(*Out[i + 4]);
			}		
			StartGameHost(port);
			ABattleBumperPlayerController* pController = Cast<ABattleBumperPlayerController>(GetWorld()->GetFirstPlayerController());
			pController->inGameID = id;

		}
}

void AMyLevelScriptActor::StartGameHost(int port)
{
	hostPort = port;
	readyToHost = true;
	ABattleBumperPlayerController* pController = Cast<ABattleBumperPlayerController>(GetWorld()->GetFirstPlayerController());
}
