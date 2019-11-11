// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class BATTLEBUMPER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	UPROPERTY(EditAnywhere)
		bool collision = false;

	UPROPERTY(EditAnywhere)
		bool collisionleft = false;

	UPROPERTY(EditAnywhere)
		bool collisionright = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Input functions
	void MoveForward(float value);
	void MoveRight(float value);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, replicated)
	class UBoxComponent* OurCollider;

	UPROPERTY(replicated)
		AActor* MyOwner;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent;
	
	UFUNCTION(BlueprintCallable, Category = "Input")
		void Handbrake();

	void ReleaseHandbrake();
	void StartGrowing();
	void StopGrowing();
	void UseBoost();

	void mouseYawn(float axis);
	void mousePitch(float axis);

	//Vector
	FVector2D mouseInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		class USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		class UCameraComponent* camera;



	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* myMesh;
	//Input variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector CurrentVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float maxVelocityX = 1200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float maxVelocityRX = -250;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float maxAccelaration = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float maxVelocityY = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float dragX = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float dragXBoost = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float dragZ = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector CurrentAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FRotator CurrentRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector HandbrakeForward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float HandbrakeNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float HandbrakeAccelaration = -3.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float HandbrakeBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiplayer")
		int PlayerID;


	bool bGrowing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool boosted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		AActor* CollidedActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		AActor* CollidedActor2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		int boost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool uHandbrake = false;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapBegin2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
		void OnOverlapBegin3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
		void OnOverlapBegin4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
		float ReturnVelocity();

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* FrontTriggerCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* BackTriggerCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* LeftTriggerCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* RightTriggerCapsule;

};
