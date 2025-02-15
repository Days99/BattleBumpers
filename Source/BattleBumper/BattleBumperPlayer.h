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
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		bool ShieldCollection = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		bool SawbladeCollection = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		bool MineCollection = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		bool GrenadeCollection = false;

	UPROPERTY(Replicated, EditAnywhere)
		bool WasHit = false;

	UPROPERTY(EditAnywhere)
		float ArrowAngle;

	UPROPERTY(Replicated,EditAnywhere)
		float AuxImpact = 1;

	UPROPERTY(replicated, EditAnywhere)
		bool HitWorld = false;

	UPROPERTY(EditAnywhere)
		bool collisionleft = false;

	UPROPERTY(EditAnywhere)
		bool collisionright = false;

	UPROPERTY(EditAnywhere)
		bool ShieldCollision = false;
	UPROPERTY(EditAnywhere)
		bool MineCollisions = false;
	UPROPERTY(Replicated,EditAnywhere)
		bool ShieldActivated = false;
	UPROPERTY(Replicated, EditAnywhere)
		bool MineDroped = false;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AMyMine> ToSpawn;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AMyGrenade> ToSpawnGrenade;
	UPROPERTY(Replicated, EditAnywhere)
		bool AddDamage = false;
	UPROPERTY(Replicated, EditAnywhere)
		bool AddDamageMine = false;
	UPROPERTY(Replicated, EditAnywhere)
		bool AddDamageShield = false;
	UPROPERTY(EditAnywhere)
		bool ItemActivated = false;

	UPROPERTY(Replicated, EditAnywhere)
		FVector CollsionVector;
	UPROPERTY(Replicated, EditAnywhere)
		FVector MineVector;
	UPROPERTY(Replicated, EditAnywhere)
		FVector ShieldVector;

	

	UPROPERTY(Replicated, EditAnywhere)
		FVector CollsionVectorWorld;
	UPROPERTY(EditAnywhere)
		FVector ColisionDirection;

	UPROPERTY(EditAnywhere)
		FHitResult HitoResulto;

	
	FVector PreviousLocation;


	UPROPERTY(Replicated, EditAnywhere)
		float ImpactStrenght;

	UPROPERTY(Replicated, EditAnywhere)
		float ImpactStrenghtWorld;

	UPROPERTY(EditAnywhere)
		float Timer;
	UPROPERTY(Replicated, EditAnywhere)
		UParticleSystem * HandbrakeEffect;
	UPROPERTY(Replicated, EditAnywhere)
		UParticleSystem* ExplosionEffect;


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
	UAudioComponent* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundEffect")
	USoundBase* EngineSoundBase;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
	UAudioComponent* ShieldHitSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
	UAudioComponent* ChainSawSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* EngineSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* ItemGetSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* BoostGetSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* ItemActivateSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* CarBumpWeakSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* CarBumpMediumSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* CarBumpStrongSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* BoostActivateSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* BoostFlameSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PLEASEWORK")
		UAudioComponent* SawSound;

	UPROPERTY(Replicated, EditAnywhere)
		FVector CurrentPosition;

	UPROPERTY(EditAnywhere)
		float TimeCounter;


	UPROPERTY(Replicated, EditAnywhere)
		float CurrentDamage;

	UPROPERTY(EditAnywhere)
		int Lives;

	UPROPERTY(EditAnywhere)
		int passage = 0;

	UPROPERTY(EditAnywhere)
		float g;
	float oMaxVelocityY;
	UPROPERTY(Replicated)
	FVector SNewLocation;
	UPROPERTY(Replicated)
	FRotator SNewRotation;


	FVector impactVector;

	float offset;
	UPROPERTY(EditAnywhere)
		FTimerHandle DelayTimer;
	UPROPERTY(EditAnywhere)
		FTimerHandle DelayTimerBoost;
	UPROPERTY(EditAnywhere)
		FTimerHandle DelayTimerMine;

	UPROPERTY(EditAnywhere)
		FTimerHandle ShieldCollisionTimer;

	FVector WallPosition;

	int timeCollied;

	UPROPERTY(EditAnywhere)
		FTimerHandle DelayTimerWorld;
	UPROPERTY(EditAnywhere)
	bool OnGround;

	float previousGroundedNormal;

	float impactStrnght;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Respawn();

	FVector GroundPosition;

	float oMaxAccelaration;

	float CameraPitch;

	float CameraYaw;

	UStaticMesh* Car1;
	UStaticMesh* Car2;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Reset();
	UPROPERTY(Replicated,EditAnywhere)
	int id;

	FVector RespawnPosition;

	class APlayerController* controller;
	
	class ABattleBumperPlayerController* MyController;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASawbladeActor> sawblade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABattleBumperPlayer* playerAssasin;

	UPROPERTY(replicated)
	AActor* MyOwner;

	class UMyGameInstance* gameInstance;

	UWorld* World;

	bool positionSet;


	int GenerateID(ABattleBumperPlayer* p);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReliableFunctionCallThatRunsOnServer(ABattleBumperPlayer* a, FVector FNewLocation, FRotator FNewRotation, float Velocity, float d, bool handbrake, bool shield, float handbrakeN);

	UFUNCTION(Server, Reliable,WithValidation)
	void Server_BumperCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght,ABattleBumperPlayer* a);

	UFUNCTION(NetMulticast, Reliable)
	void Client_BumperCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght, ABattleBumperPlayer* a);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTo_BumperCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght, ABattleBumperPlayer* a);

	UFUNCTION(NetMulticast, Reliable)
		void ClientTo_BumperCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght, ABattleBumperPlayer* a);

	UFUNCTION(NetMulticast, Reliable)
	void Client_ReliableFunctionCallThatRunsOnOwningClientOnly(ABattleBumperPlayer* a, FVector FNewLocation, FRotator FNewRotation, float v, float d, bool handbrake, bool shield, float handbrakeN);

	void UpdateClients(ABattleBumperPlayer* a);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere)
	USceneComponent* OurVisibleComponent;
	//Input functions
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Handbrake();

	void ShieldHit(FVector NImpactNormal);
	void ReleaseHandbrake();
	void StartGrowing();
	void StopGrowing();
	void UseBoost();
	UFUNCTION(NetMulticast, Reliable)
	void SpawnMine(ABattleBumperPlayer* a);
	UFUNCTION(Server, Reliable,WithValidation)
		void Server_SpawnMine(ABattleBumperPlayer* a);
	UFUNCTION(NetMulticast, Reliable)
		void SpawnSawblade(ABattleBumperPlayer* a);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SpawnSawblade(ABattleBumperPlayer* a);

	UFUNCTION(NetMulticast, Reliable)
		void SpawnGrenade(ABattleBumperPlayer* a);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SpawnGrenade(ABattleBumperPlayer* a);

	UPROPERTY(BlueprintReadWrite)
	bool winner;


	void DestroySawblade();
	void DestroyShield();
	void ActivateItem();
	void ActivateShield();
	void ShieldTimer();
	void CollisionFalse();
	void BoostFalse();
	void CollisionWorldFalse();
	void BeginShieldTimer();
	void MineFalse();
	void WorldCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght);
	void MineCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght);
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
	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	FTimerHandle SawbladeTime;

	AActor* sawbladeActor;

	bool sawbladeActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* myMesh;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShieldMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UStaticMeshComponent* BoostEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UParticleSystemComponent* BoostsRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UParticleSystemComponent* BoostsLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UParticleSystemComponent* NormalBoostRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UParticleSystemComponent* NormalBoostLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UParticleSystemComponent* UnderBoostRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UParticleSystemComponent* UnderBoostLeft;
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
	float YourVelocityShield;
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
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FRotator CurrentRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector HandbrakeForward;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Movement")
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
	FTimerHandle ShieldTime;
	FTimerHandle ShieldTimeForce;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector GroundedForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FRotator GroundRotation;

	bool bGrowing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool boosted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool boosting;
	UPROPERTY(replicated, EditAnywhere, BlueprintReadWrite, Category = "Collision")
	ABattleBumperPlayer* CollidedActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		AActor* CollidedActor2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int boost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool onHandbrake;
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

	UFUNCTION()
		void OnOverlapBeginShield(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEndShield(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

	UFUNCTION(BlueprintCallable)
		float ReturnVelocity();


	UFUNCTION(BlueprintCallable)
		float ReturnVelocityRotation();

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

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class USphereComponent* ShieldCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule", replicated)
	class UBoxComponent* OurCollider;


};
