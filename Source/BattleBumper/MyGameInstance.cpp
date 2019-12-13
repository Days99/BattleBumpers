// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Net/UnrealNetwork.h"

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

void UMyGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty, FDefaultAllocator> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UGameInstance, this);
}

void UMyGameInstance::StartGame() {
	if (players.Num() == maxPlayers) {
		for (ABattleBumperPlayer* p : players) {
			p->Reset();
		}
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


