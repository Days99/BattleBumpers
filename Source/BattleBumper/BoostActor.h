// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoostActor.generated.h"

UCLASS()
class BATTLEBUMPER_API ABoostActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoostActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnCollided();

	void OnRespawn();

	bool active;


	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PillMesh;

	FTimerHandle respawningTime;


	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
		FRotator Rotator;


};
