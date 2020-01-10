// Fill out your copyright notice in the Description page of Project Settings.


#include "SawbladeActor.h"

// Sets default values
ASawbladeActor::ASawbladeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SawbladeMesh"));

	RootComponent = Root;

	Mesh->SetupAttachment(RootComponent);
	rotation.Yaw = 10;


}

// Called when the game starts or when spawned
void ASawbladeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASawbladeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Mesh->AddRelativeRotation(rotation);
	
}

