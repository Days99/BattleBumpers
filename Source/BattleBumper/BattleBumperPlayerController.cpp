// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBumperPlayerController.h"

void ABattleBumperPlayerController::BeginPlay() 
{
	pawn = Cast<ABattleBumperPlayer>(GetPawn());
//	pawnDestroyed.BindStatic(pawn->OnDestroyed, UWorld::SpawnActor(class ABattleBumperPlayer *,FVector::ZeroVector,FActorSpawnParameters::FActorSpawnParameters());

}