// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRespawnActor.h"

// Sets default values
AMyRespawnActor::AMyRespawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyRespawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyRespawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

