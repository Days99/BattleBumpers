// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGrenade.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyGrenade::AMyGrenade()
{
	myMesh = CreateDefaultSubobject<USphereComponent>(TEXT("MESH"));
	anotherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH2"));
	anotherMesh->SetSimulatePhysics(true);
	//EngineSound->SetSound(EngineSoundBase);
	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionAudioComp"));
	// I don't want the sound playing the moment it's created.
	ExplosionSound->bAutoActivate = false;

	RootComponent = anotherMesh;
	myMesh->SetupAttachment(RootComponent);
	GrenadeActivated = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyGrenade::BeginPlay()
{
	
	Super::BeginPlay();

	anotherMesh->SetPhysMaterialOverride(PhysMat);
	GetWorld()->GetTimerManager().SetTimer(DelayTimerSound, this, &AMyGrenade::ActivateSound, 2.5f, false);
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &AMyGrenade::ActivateGrenade, 3.0f, false);

	anotherMesh->AddImpulse(GetActorForwardVector() * 400000, NAME_None, false);

}

// Called every frame
void AMyGrenade::Tick(float DeltaTime)
{	

	Super::Tick(DeltaTime);
}
void AMyGrenade::ActivateSound()
{
	if (!ExplosionSound->IsPlaying())
		ExplosionSound->Play();
}
void AMyGrenade::ActivateGrenade()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	Destroy();
}
