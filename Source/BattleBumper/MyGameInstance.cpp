// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

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

void UMyGameInstance::RemovePlayer(ABattleBumperPlayer* p)
{
	if (players.Contains(p)) {
		players.Remove(p);
	}
}


