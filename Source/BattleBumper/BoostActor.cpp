// Fill out your copyright notice in the Description page of Project Settings.

#include "BoostActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABoostActor::ABoostActor()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;
	Rotator.Yaw = 2;
	PillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	PillMesh->SetupAttachment(RootComponent);
	active = true;

}

// Called when the game starts or when spawned
void ABoostActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABoostActor::OnCollided()
{
	active = false;
	PillMesh->SetVisibility(false);
	GetWorld()->GetTimerManager().SetTimer(respawningTime, this, &ABoostActor::OnRespawn, 5.0f, false);

}

void ABoostActor::OnRespawn()
{
	active = true;
	PillMesh->SetVisibility(true);

}
// Called every frame
void ABoostActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PillMesh->AddRelativeRotation(Rotator);
}


