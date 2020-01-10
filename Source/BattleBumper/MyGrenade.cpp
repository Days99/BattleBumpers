// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGrenade.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMyGrenade::AMyGrenade()
{
	myMesh = CreateDefaultSubobject<USphereComponent>(TEXT("MESH"));
	anotherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH2"));
	anotherMesh->SetSimulatePhysics(true);
	
	RootComponent = anotherMesh;
	myMesh->SetupAttachment(RootComponent);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyGrenade::BeginPlay()
{
	
	Super::BeginPlay();
	anotherMesh->SetPhysMaterialOverride(PhysMat);
	GrenadeActivated = false;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &AMyGrenade::ActivateGrenade, 0.5f, false);
	anotherMesh->AddImpulse(GetActorForwardVector() * 60000, NAME_None, false);

}

// Called every frame
void AMyGrenade::Tick(float DeltaTime)
{	
	
	Super::Tick(DeltaTime);
}

void  AMyGrenade::ActivateGrenade()
{
	GrenadeActivated = true;
}
