// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Engine//Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"
#include "UserWidget.h"
#include "Components/Button.h"
#include "Net/UnrealNetwork.h"

//
//void UMyGameInstance::Init() 
//{
//	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
//
//	if (Subsystem) {
//		SessionInterface = Subsystem->GetSessionInterface();
//		if (SessionInterface.IsValid()) {
//			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);
//			SessionInterface->OnStartSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnStartSessionComplete);
//			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnDestroySessionComplete);
//			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyGameInstance::OnFindSessionsComplete);
//			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnJoinSessionComplete);
//
//			SearchSession();
//		}
//	}
//
//}
//
//void UMyGameInstance::JoinSession()
//{
//	if (SessionSearch.IsUnique()) {
//		if (SessionSearch->SearchResults.Num() > 0) {
//			SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[0]);
//		}
//	}
//}
//void UMyGameInstance::SearchSession()
//{
//	SessionSearch = MakeShareable(new FOnlineSessionSearch());
//
//	if (SessionSearch.IsUnique()) {
//		SessionSearch->bIsLanQuery = true;
//		SessionInterface->FindSessions(0, SessionSearch);
//	}
//}
//void UMyGameInstance::Host()
//{
//	if (SessionInterface.IsValid()) {
//		FNamedOnlineSession* existingSession = SessionInterface->GetNamedSession(SESSION_NAME);
//
//		if(existingSession){
//			DestroySession();
//		}
//		else {
//			CreateSession();
//		}
//		CreateSession();
//
//	}
//}
//void UMyGameInstance::Join()
//{
//	JoinSession();
//}
//void UMyGameInstance::CreateSession()
//{
//	FOnlineSessionSettings Settings;
//	Settings.bIsLANMatch = true;
//	Settings.bShouldAdvertise = true;
//	Settings.NumPublicConnections = 8;
//
//	SessionInterface->CreateSession(0,SESSION_NAME, Settings);
//	//FNamedOnlineSession* existingSession = SessionInterface
//}
//void UMyGameInstance::DestroySession()
//{
//}
//void UMyGameInstance::StartSession()
//{
//}
//void UMyGameInstance::DestroySession()
//{
//}
//void UMyGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
//
//}
//
//void UMyGameInstance::OnFindSessionsComplete(bool bWasSuccessful) {
//	if (bWasSuccessful && SessionSearch.IsUnique) {
//		for (int i = 0; i < SessionSearch->SearchResults.Num(); i++) {
//			FOnlineSessionSearchResult searchResult = SessionSearch->SearchResults[i];
//		}
//	}
//
//}
//
//void UMyGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {
//
//}
//
//void UMyGameInstance::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
//{
//}
//
//void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type type)
//{
//}

void UMyGameInstance::AddPlayer(ABattleBumperPlayer* p) {
	int count = 1;
	for (ABattleBumperPlayer* pawn : players) {
		if (pawn == p)
			break;
		else
			count++;
	}

	if (count > players.Num()) {
		players.Add(p);
	}
}

int UMyGameInstance::GenerateID(ABattleBumperPlayer* p)
{
	int count = 1;
	for (ABattleBumperPlayer* pawn : players) {
		if (pawn == p)
			break;
		else
			count++;
	}




	if (count > players.Num()) {
		players.Add(p);
		return count;
	}
	return p->id;
}
//
//void UMyGameInstance::LoadMenu() {
//	if (MenuWidgetClass)
//	{
//		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
//		MenuWidget->AddToViewport();
//		UButton* connectButton = Cast<UButton>(MenuWidget->GetWidgetFromName(TEXT("HostButton")));
//		if (connectButton)
//		{
//			connectButton->OnClicked.AddDynamic(this, &UMyGameInstance::CreateSession);
//		}
//		UButton* hostButton = Cast<UButton>(MenuWidget->GetWidgetFromName(TEXT("JoinButton")));
//		if (hostButton)
//		{
//			hostButton->SetIsEnabled(false);
//			hostButton->OnClicked.AddDynamic(this, &UMyGameInstance::Host);
//		}
//	}
//}


void UMyGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty, FDefaultAllocator> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UGameInstance, this);
}

void UMyGameInstance::StartGame() {
		for (ABattleBumperPlayer* p : players) {
			p->Reset();
		}
}

ABattleBumperPlayer* UMyGameInstance::GetRandomPlayer()
{
	int i = rand() % players.Num();
	return players[i];
}


void UMyGameInstance::SetMaxPlayers(int max) {
	maxPlayers = max;
}

int UMyGameInstance::GetMaxPlayers()
{
	return maxPlayers;
}

void UMyGameInstance::ResetGame()
{
	for (ABattleBumperPlayer* p : players) {
		p->Reset();
	}
}

void UMyGameInstance::ClearPlayers()
{
	players.Empty();
}

void UMyGameInstance::RemovePlayer_Implementation(ABattleBumperPlayer* p)
{
	if (players.Contains(p)) {
		players.Remove(p);
	}
	if (players.Num() >= 1) {
		players[0]->winner = true;
	}
}


