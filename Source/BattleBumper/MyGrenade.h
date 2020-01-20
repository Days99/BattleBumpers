// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGrenade.generated.h"


UCLASS()
class BATTLEBUMPER_API AMyGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGrenade();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* myMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* anotherMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* ExplosionSound;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere)
	UPhysicalMaterial* PhysMat;

	UPROPERTY(EditAnywhere)
		FTimerHandle DelayTimer;

	UPROPERTY(EditAnywhere)
		FTimerHandle DelayTimerSound;

	UPROPERTY(EditAnywhere)
		bool GrenadeActivated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ActivateGrenade();
	void ActivateSound();
};
