// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "BattleBumperPlayer.h"
#include "GameFramework/Pawn.h"


float AMyPlayerState::ReturnPlayerVelocity() {
	ABattleBumperPlayer *  p = Cast<ABattleBumperPlayer>(GetOwner());
	return p->CurrentVelocity.X;
}