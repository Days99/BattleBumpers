// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "BattleBumperPlayer.generated.h"


UCLASS()
class BATTLEBUMPER_API ABattleBumperPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABattleBumperPlayer();
	UPROPERTY(EditAnywhere)
		bool collision = false;

	UPROPERTY(replicated, EditAnywhere)
		bool WasHit = false;

	UPROPERTY(replicated, EditAnywhere)
		bool HitWorld = false;

	UPROPERTY(EditAnywhere)
		bool collisionleft = false;

	UPROPERTY(EditAnywhere)
		bool collisionright = false;

	UPROPERTY(Replicated, EditAnywhere)
		bool AddDamage = false;

	UPROPERTY(Replicated, EditAnywhere)
		FVector CollsionVector;

	UPROPERTY(Replicated, EditAnywhere)
		FVector CollsionVectorWorld;

	UPROPERTY(Replicated, EditAnywhere)
		float ImpactStrenght;

	UPROPERTY(Replicated, EditAnywhere)
		float ImpactStrenghtWorld;

	UPROPERTY(EditAnywhere)
		float Timer;

	UPROPERTY(EditAnywhere)
		float TimeCounter;


	UPROPERTY(Replicated, EditAnywhere)
		float CurrentDamage;

	UPROPERTY(EditAnywhere)
		int Lives;
	float oMaxVelocityY;

	UPROPERTY(EditAnywhere)
		FTimerHandle DelayTimer;

	UPROPERTY(EditAnywhere)
		FTimerHandle DelayTimerWorld;

	float previousGroundedNormal;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Respawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, replicated)
	UBoxComponent* OurCollider;

	UPROPERTY(EditAnywhere, replicated)
	UCharacterMovementComponent* MovementCharacter;

	UPROPERTY(replicated)
	AActor* MyOwner;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReliableFunctionCallThatRunsOnServer(ABattleBumperPlayer* a, FVector NewLocation, FRotator NewRotation, float Velocity, float d);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_BumperCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght);

	UFUNCTION(NetMulticast, Reliable)
		void Client_ReliableFunctionCallThatRunsOnOwningClientOnly(ABattleBumperPlayer* a, FVector NewLocation, FRotator NewRotation, float v, float d);

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere)
	USceneComponent* OurVisibleComponent;
	//Input functions
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Handbrake();

	void ReleaseHandbrake();
	void StartGrowing();
	void StopGrowing();
	void UseBoost();
	void CollisionFalse();
	void CollisionWorldFalse();
	void WorldCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght);
	void BumperCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght);

	void mouseYawn(float axis);
	void mousePitch(float axis);

	UFUNCTION(BlueprintCallable)
		void CalculateSlopeRotation();
	//Vector
	FVector2D mouseInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UCameraComponent* camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DistanceZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float currentDistanceZ;

	float locationZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector CollisionTreshold;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* myMesh;
	//Input variables
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector CurrentVelocity;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector ServerVelocity;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int Grounded;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GroundedNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GroundedRotationValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
		FTransform respawnTransform;
	bool respawning;
	FTimerHandle respawningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector GroundedForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FRotator GroundRotation;

	bool bGrowing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool boosted;
	UPROPERTY(replicated, EditAnywhere, BlueprintReadWrite, Category = "Collision")
	ABattleBumperPlayer* CollidedActor;
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


	UFUNCTION()
		void OnOverlapBeginGround(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEndGround(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	

	UFUNCTION(BlueprintCallable)
		float ReturnVelocity();

	UFUNCTION(BlueprintCallable)
		float ReturnDamage();


	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* FrontTriggerCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* BackTriggerCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* LeftTriggerCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* RightTriggerCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* GroundCapsule;


};
