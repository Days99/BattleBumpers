// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuLevelScript.h"
#include "Button.h"

void AMenuLevelScript::BeginPlay()
{
	Super::BeginPlay();

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

void AMenuLevelScript::OnConnectClicked()
{
	TCPclienty = new TCPClient(this);
}
void AMenuLevelScript::OnHostClicked()
{
}