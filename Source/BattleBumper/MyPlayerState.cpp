// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "BattleBumperPlayer.h"
#include "GameFramework/Pawn.h"


float AMyPlayerState::ReturnPlayerVelocity(ABattleBumperPlayer * pawn) {
	return pawn->CurrentVelocity.X;
}

float AMyPlayerState::ReturnPlayerDamage(ABattleBumperPlayer* pawn) {
	return pawn->CurrentDamage;
}