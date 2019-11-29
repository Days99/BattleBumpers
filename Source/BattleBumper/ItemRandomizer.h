// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ItemRandomizer.generated.h"

UCLASS()
class BATTLEBUMPER_API AItemRandomizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemRandomizer();

	bool PlayerCollided;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		class UBoxComponent *ItemCollider;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *QuestionMarkMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		UStaticMeshComponent* SpherekMesh;
	UPROPERTY(EditAnywhere)
	FRotator Rotator;

	FTimerHandle respawningTime;

	void Respawn();
	void TimerFunc();
};
