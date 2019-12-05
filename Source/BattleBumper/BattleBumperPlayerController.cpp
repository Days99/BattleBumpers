// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBumperPlayerController.h"

void ABattleBumperPlayerController::BeginPlay() 
{
	pawn = Cast<ABattleBumperPlayer>(GetPawn());
//	pawnDestroyed.BindStatic(pawn->OnDestroyed, UWorld::SpawnActor(class ABattleBumperPlayer *,FVector::ZeroVector,FActorSpawnParameters::FActorSpawnParameters());

}


void ABattleBumperPlayerController::Server_ReliableFunctionCallThatRunsOnServer_Implementation(ABattleBumperPlayer* a, FVector NewLocation, FRotator NewRotation, float v, float d, bool handbrake, bool shield)
{
	a->SetActorLocationAndRotation(NewLocation, NewRotation, true);
	a->SNewLocation = a->GetActorLocation();
	a->SNewRotation = a->GetActorRotation();
	a->CurrentPosition = a->GetActorLocation();
	a->ServerVelocity.X = v;
	a->CurrentDamage = d;
	a->onHandbrake = handbrake;
	a->ShieldActivated = shield;
}
bool ABattleBumperPlayerController::Server_ReliableFunctionCallThatRunsOnServer_Validate(ABattleBumperPlayer* a, FVector NewLocation, FRotator NewRotation, float v, float d, bool handbrake, bool shield)
{
	return true;
}
