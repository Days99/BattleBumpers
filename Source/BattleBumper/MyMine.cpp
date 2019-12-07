// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMine.h"

// Sets default values
AMyMine::AMyMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyMine::BeginPlay()
{
	Super::BeginPlay();
	MineActivated = false;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &AMyMine::ActivateMine, 2.0f, false);
}

// Called every frame
void AMyMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyMine::ActivateMine()
{
	MineActivated = true;
}

