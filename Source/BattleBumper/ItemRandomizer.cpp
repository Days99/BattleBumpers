// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemRandomizer.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AItemRandomizer::AItemRandomizer()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	QuestionMarkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//ItemCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("SHIELDS"));
	SpherekMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SPHEREMESHO"));
	RootComponent = SpherekMesh;
	//ItemCollider->SetupAttachment(RootComponent);
	QuestionMarkMesh->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void AItemRandomizer::BeginPlay()
{
	Super::BeginPlay();
	PlayerCollided = false;
}

// Called every frame
void AItemRandomizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerCollided)
	{
		QuestionMarkMesh->SetVisibility(false, false);
		SpherekMesh->SetVisibility(false, false);
	}
	Rotator.Yaw += 1;
	QuestionMarkMesh->SetRelativeRotation(Rotator);
}

void AItemRandomizer::Respawn()
{
	PlayerCollided = false;
	QuestionMarkMesh->SetVisibility(true, false);
	SpherekMesh->SetVisibility(true, false);
}

void AItemRandomizer::TimerFunc()
{
	GetWorld()->GetTimerManager().SetTimer(respawningTime, this, &AItemRandomizer::Respawn, 20.0f, false);
}