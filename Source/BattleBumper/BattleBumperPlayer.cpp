// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBumperPlayer.h"
#include "Components/PrimitiveComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Particles/ParticleSystem.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "math.h"
#include "GroundActor.h"
#include "DeathZoneActor.h"
#include "WallActor.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "ItemRandomizer.h"
#include "BoostActor.h"
#include "EngineUtils.h"
#include "MyGameInstance.h"
#include "MyRespawnActor.h"


// Sets default values
ABattleBumperPlayer::ABattleBumperPlayer()
{
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyOwner = GetOwner();
	respawnTransform = GetActorTransform();
	Lives = 3;
	
	// Create a dummy root component we can attach things to.
	
	// Create a camera and a visible object
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SHIELDS"));
	//MovementCharacter = CreateDefaultSubobject<UCharacterMovementComponent>((TEXT("Movement")));

	bReplicates = true;
	SetReplicateMovement(true);
	OurCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("OurCollider"));

	if (Role == ROLE_Authority)
		World = GetWorld();


	static ConstructorHelpers::FObjectFinder<UStaticMesh>Car1Asset(TEXT("/Game/Assets/NewBattleBumper"));
	Car1 = Car1Asset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Car2Asset(TEXT("/Game/Assets/BattleBumperThin"));
	Car2 = Car2Asset.Object;


	

	//ServerMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("ServerMovement"));

	OurCollider->SetNotifyRigidBodyCollision(true);

	RootComponent = OurCollider;
	

	//ServerMovement->AddToRoot();
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = 450.0;

	if (Role != ROLE_Authority)
		Role = ROLE_AutonomousProxy;
	
	

	
	myMesh->SetupAttachment(RootComponent);
	ShieldMesh->SetupAttachment(RootComponent);
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);

	
	
	GroundCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Ground Capsule"));
	GroundCapsule->SetupAttachment(RootComponent);

	
	// declare trigger capsule
	FrontTriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	FrontTriggerCapsule->InitCapsuleSize(30.0f, 70.0f);
	FrontTriggerCapsule->SetWorldLocation(FVector(140.f, 0, -10.f));
	FrontTriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	FrontTriggerCapsule->SetupAttachment(RootComponent);
	
	// declare trigger capsule
	BackTriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule 2"));
	BackTriggerCapsule->InitCapsuleSize(55.f, 96.0f);
	BackTriggerCapsule->SetCollisionProfileName(TEXT("Trigger 2"));
	BackTriggerCapsule->SetupAttachment(RootComponent);

	// declare trigger capsule
	LeftTriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule 3"));
	LeftTriggerCapsule->InitCapsuleSize(55.f, 96.0f);
	LeftTriggerCapsule->SetCollisionProfileName(TEXT("Trigger 3"));
	LeftTriggerCapsule->SetupAttachment(RootComponent);

	// declare trigger capsule
	RightTriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule 4"));
	RightTriggerCapsule->InitCapsuleSize(55.f, 96.0f);
	RightTriggerCapsule->SetCollisionProfileName(TEXT("Trigger 4"));
	RightTriggerCapsule->SetupAttachment(RootComponent);

	// declare trigger capsule
	ShieldCapsule = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldCapsule"));
	ShieldCapsule->InitSphereRadius(55.0f);
	ShieldCapsule->SetCollisionProfileName(TEXT("ShieldTrigger"));
	ShieldCapsule->SetupAttachment(RootComponent);


}

void ABattleBumperPlayer::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABattleBumperPlayer, MyOwner);
	DOREPLIFETIME(ABattleBumperPlayer, SNewLocation);
	DOREPLIFETIME(ABattleBumperPlayer, SNewRotation);



}
// Called when the game starts or when spawned
void ABattleBumperPlayer::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<UMyGameInstance>(GetGameInstance());
	id = gameInstance->GenerateID(this);

	if(id == 0 || id == 2)
		myMesh->SetStaticMesh(Car1);
	else
		myMesh->SetStaticMesh(Car2);

	TSubclassOf<AMyRespawnActor> classToFind;
	classToFind = AMyRespawnActor::StaticClass();
	TArray<AActor*> foundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, foundObjects);

	for (AActor* a : foundObjects) {
		AMyRespawnActor* respawn = Cast<AMyRespawnActor>(a);
		if (respawn && respawn->id == id) {
			RespawnPosition = respawn->GetActorLocation();
		}
	}



	ShieldMesh->SetVisibility(false);
	oMaxVelocityY = maxVelocityY;
	oMaxAccelaration = maxAccelaration;
	respawnTransform = GetActorTransform();
	CurrentPosition = GetActorLocation();
	controller = Cast<APlayerController>(GetController());



	FRotator OriginalCameraRotation = FRotator(-20, 0, 0);
	springArm->AddRelativeRotation(OriginalCameraRotation);

	FrontTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapBegin);
	FrontTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapEnd);

	BackTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapBegin2);
	BackTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapEnd2);

	LeftTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapBegin3);
	LeftTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapEnd3);
	
	RightTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapBegin4);
	RightTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapEnd4);

	GroundCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapBeginGround);
	GroundCapsule->OnComponentEndOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapEndGround);

	ShieldCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapBeginShield);
	ShieldCapsule->OnComponentEndOverlap.AddDynamic(this, &ABattleBumperPlayer::OnOverlapEndShield);

	OurCollider->OnComponentHit.AddDynamic(this, &ABattleBumperPlayer::OnCompHit);
	gameInstance->StartGame();

}

// Called every frame
void ABattleBumperPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle growing and shrinking based on our "Grow" action
	if (collision == true)
	{
		CurrentVelocity.X = 0;
		CurrentAcceleration.X = 0;
		//WasHit = false;
		collision = false;
	}

	if (collisionright == true)
	{
		CurrentVelocity.X = 0;
		CurrentAcceleration.X = 0;
		if (CurrentAcceleration.Y >= 0)
		{
			CurrentAcceleration.Y = 0;
		}
	}

	if (collisionleft == true)
	{
		CurrentVelocity.X = 0;
		CurrentAcceleration.X = 0;
		if (CurrentAcceleration.Y <= 0)
		{
			CurrentAcceleration.Y = 0;

		}
	
		
	}

	if (CurrentAcceleration.X == 0) {
		if (CurrentVelocity.X >= dragX)
		{
			CurrentAcceleration.X -= dragX;
		}
		if (CurrentVelocity.X <= -dragX)
		{
			CurrentAcceleration.X += dragX;
		}
		if (CurrentVelocity.X > -dragX && CurrentVelocity.X < dragX) {
			CurrentVelocity.X = 0;
		}
	}
	if (CurrentVelocity.X >= dragXBoost && boosted) {
		CurrentAcceleration.X -= dragXBoost;
	}
	if (CurrentRotation.Yaw > dragZ && CurrentAcceleration.Y == 0)
	{
		CurrentAcceleration.Y -= dragZ;
	}
	if (CurrentRotation.Yaw < -dragZ && CurrentAcceleration.Y == 0)
	{
		CurrentAcceleration.Y += dragZ;
	}

	if (CurrentVelocity.X == 0) {
		maxVelocityY = oMaxVelocityY /4;
	}
	else {
		maxVelocityY = oMaxVelocityY;
	}
	if (!onHandbrake && !respawning)
		CurrentVelocity.X += CurrentAcceleration.X;
	
	if (onHandbrake && CurrentVelocity.X > 0) {
		if (CurrentVelocity.X < HandbrakeAccelaration) {
			CurrentVelocity.X = 0;
		}
		else
		CurrentVelocity.X += HandbrakeAccelaration;
	}
	else if (onHandbrake && CurrentVelocity.X < 0) {
		if (CurrentVelocity.X > HandbrakeAccelaration) {
			CurrentVelocity.X = 0;
		}
		else
		CurrentVelocity.X += HandbrakeAccelaration;
	}

	if (CurrentAcceleration.Y == 0) {
		CurrentRotation.Add(0, -CurrentRotation.Yaw, 0);
	}
	else  
	CurrentRotation.Add(0, CurrentAcceleration.Y / 10,0);

	if (CurrentVelocity.X > maxVelocityX && CurrentAcceleration.X > 0 && !boosted) 
	{
		CurrentVelocity.X = maxVelocityX;
	}

	if (CurrentVelocity.X < maxVelocityRX && CurrentAcceleration.X < 0)
	{
		CurrentVelocity.X = maxVelocityRX;
	}
	
	if(CurrentRotation.Yaw > maxVelocityY && CurrentAcceleration.Y > 0 && !onHandbrake )
	{
		CurrentRotation.Yaw = maxVelocityY;
	}
	else if (onHandbrake && CurrentRotation.Yaw > maxVelocityY * 1.5f) {
		CurrentRotation.Yaw = maxVelocityY * 1.5f;
	}
	
	if (CurrentRotation.Yaw < -maxVelocityY && CurrentAcceleration.Y < 0 && !onHandbrake)
	{
		CurrentRotation.Yaw = -maxVelocityY;
	}
	else if (onHandbrake && CurrentRotation.Yaw < -maxVelocityY * 1.5f) {
		CurrentRotation.Yaw = -maxVelocityY * 1.5f;
	}
	if (boosted && CurrentVelocity.X < maxVelocityX) {
		boosted = false;
	}




	// Handle movement based on our "MoveX" and "MoveY" axes

	//if (!CurrentVelocity.IsZero() || !CurrentRotation.IsZero() || Grounded >= 0 || collision)
	//{

		FRotator NewRotation = GetActorRotation() + (CurrentRotation * DeltaTime);

		FVector NewLocation = GetActorLocation();
		if (!WasHit)
			PreviousLocation = NewLocation;
		//	CollisionTreshold = NewLocation;
		if (!respawning) {
			if (onHandbrake && CurrentVelocity.X != 0) {
				NewLocation += GetActorForwardVector() * (1000 * (CurrentVelocity.X /maxVelocityX)) * DeltaTime;
			}
			else if(ServerVelocity.X != 0)
			NewLocation += GetActorForwardVector() * CurrentVelocity.X * DeltaTime;



			if (Grounded > 0) {
				CalculateSlopeRotation();

				float difference;
				if (NewRotation.Pitch < GroundRotation.Pitch) {
					NewRotation.Pitch += (100 + GroundedRotationValue * 10) * DeltaTime;
					difference = GroundRotation.Pitch - NewRotation.Pitch;
				}
				else if (NewRotation.Pitch > GroundRotation.Pitch) {
					NewRotation.Pitch -= (100 + GroundedRotationValue * 10) * DeltaTime;
					difference = NewRotation.Pitch - GroundRotation.Pitch;
				}
				if (difference < 1)
					NewRotation.Pitch = GroundRotation.Pitch;
			}
			else if (NewRotation.Pitch > -20 && Grounded == 0)
				NewRotation.Pitch -= (30 + (30 * CurrentVelocity.X / maxVelocityX)) * DeltaTime;

			if (Grounded <= 0) {
				NewLocation += (GetActorUpVector() * -600) * DeltaTime;
			}
			float impactStrnght = 0;
			if (WasHit&&collision==false)
			{
				impactStrnght = (ImpactStrenght / 500) * (CurrentDamage / 2);
				if (ImpactStrenght >= 2000 )
				{
					NewLocation +=( (CollsionVector * ((ImpactStrenght + CurrentDamage) *1.3f))  + (GetActorUpVector() * ((ImpactStrenght + CurrentDamage)/1.3f))) * DeltaTime;
					NewRotation.Yaw += CollsionVector.Rotation().Yaw / 50;
				}
				else if (ImpactStrenght >= 1500 )
				{
					NewLocation +=( (CollsionVector * ((ImpactStrenght + CurrentDamage) * 1.1f))  + (GetActorUpVector() * ((ImpactStrenght + CurrentDamage)/2) )) * DeltaTime;
					NewRotation.Yaw += CollsionVector.Rotation().Yaw / 55;
				}
				else if (ImpactStrenght >= 1000)
				{
					NewLocation +=( (CollsionVector * ((ImpactStrenght + CurrentDamage))) + (GetActorUpVector() * ((ImpactStrenght + CurrentDamage)/1.5f)) )* DeltaTime;
					NewRotation.Yaw += CollsionVector.Rotation().Yaw / 60;
				}
				else if (ImpactStrenght < 999)
				{
					NewLocation += (CollsionVector * ((ImpactStrenght + CurrentDamage) )) * DeltaTime;
					NewRotation.Yaw += CollsionVector.Rotation().Yaw / 70;
				}
			}
			if (AddDamage)
			{
				CurrentDamage += ImpactStrenght * DeltaTime;;
				AddDamage = false;
			}
			if(AddDamageShield)
			{
				CurrentDamage += YourVelocityShield * DeltaTime;;
				AddDamageShield = false;
			}
			if (HitWorld)
			{
				if (WasHit)
				{
					NewLocation -= ((-CollsionVectorWorld * 500) + CollsionVector * ((ImpactStrenght + CurrentDamage)) + (-GetActorUpVector() * ((ImpactStrenght + CurrentDamage)/3.0f))) * DeltaTime;
				}
				NewLocation += (CollsionVectorWorld * 500) * DeltaTime;
				
			}

			if (onHandbrake && !CurrentVelocity.IsZero() && Grounded > 0 && !collision) {
				HandbrakeNormal = FVector::DotProduct(GetActorForwardVector(), HandbrakeForward);
				if (HandbrakeNormal < 0)
					HandbrakeNormal *= -1;

				if (HandbrakeNormal < 0.30) {
					FVector EffectPosition = GetActorLocation() + FVector(0, 0, -50);
					FVector EffectPosition2 = GetActorLocation() + FVector(0, 80, -50);

					UGameplayStatics::SpawnEmitterAtLocation(World, HandbrakeEffect, EffectPosition);
					UGameplayStatics::SpawnEmitterAtLocation(World, HandbrakeEffect, EffectPosition2);

				}
				if (HandbrakeNormal < 0.1) {

					HandbrakeNormal = 0.30;
				}
				NewLocation += (HandbrakeForward * (2500 * (CurrentVelocity.X / maxVelocityX))) * DeltaTime;
			}
			if(ShieldCollision)
			{
				if(YourVelocityShield > 1000)
				NewLocation += ((ShieldVector*1.3f*(YourVelocityShield+CurrentDamage)) + (GetActorUpVector() * (YourVelocityShield + CurrentDamage) / 1.5f))*DeltaTime;
				else
				NewLocation += (ShieldVector*2*(YourVelocityShield + CurrentDamage)) * DeltaTime;
				
				CurrentAcceleration.X = 0;
			}
			if (currentDistanceZ < DistanceZ && OnGround) {
				NewLocation.Z = GetActorLocation().Z + locationZ;
				NewRotation.Pitch = 0;
			}
		}
		if (respawning)
			CurrentDamage = 0;
		if (ShieldActivated)
		{
			ShieldMesh->SetVisibility(true);
			ShieldCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
		{
			ShieldCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ShieldMesh->SetVisibility(false);
		}
		FVector PreviousLocation = GetActorLocation();
		FVector ClientLocation;
		FRotator ClientRotation;
		Server_ReliableFunctionCallThatRunsOnServer(this, NewLocation, NewRotation, CurrentVelocity.X, CurrentDamage, onHandbrake, ShieldActivated);
		if (Role == ROLE_AutonomousProxy) {
			SetActorLocationAndRotation(NewLocation, NewRotation, true);
		}
	

		//if (Role == ROLE_AutonomousProxy) {
		//	SetActorLocationAndRotation(NewLocation, NewRotation, true);
		//}
		//else if (this->Role == ROLE_Authority) {
		//	Client_ReliableFunctionCallThatRunsOnOwningClientOnly(this, NewLocation, NewRotation, CurrentVelocity.X, CurrentDamage, onHandbrake, ShieldActivated);
		//}
		//
		//if (this->Role == ROLE_AutonomousProxy) {
		//	ClientLocation = GetActorLocation();
		//	ClientRotation = GetActorRotation();
		//	Client_ReliableFunctionCallThatRunsOnOwningClientOnly(this, ClientLocation, ClientRotation, CurrentVelocity.X, CurrentDamage, onHandbrake, ShieldActivated);
		//}
	//		Move_ClientMulticast(this, NewLocation, NewRotation, CurrentVelocity.X, CurrentDamage, onHandbrake, ShieldActivated);

		//if (Role == ROLE_AutonomousProxy)
		//	ClientMove();
		//if(FVector::Dist(PreviousLocation, GetActorLocation()) > 1)

		FRotator newPitch = springArm->GetComponentRotation();
		newPitch.Yaw += mouseInput.X;
		CameraYaw += mouseInput.X;
		if (CameraYaw > 0 && mouseInput.X == 0)
		{
			newPitch.Yaw -= 1;
			CameraYaw -= 1;
		}
		if (CameraYaw < 0 && mouseInput.X == 0)
		{
			newPitch.Yaw += 1;
			CameraYaw += 1;
		}
		newPitch.Pitch += mouseInput.Y;
		CameraPitch += mouseInput.Y;
		if (CameraPitch > 0 && mouseInput.Y == 0)
		{
			newPitch.Pitch -= 1;
			CameraPitch -= 1;
		}
		if (CameraPitch < 0 && mouseInput.Y == 0)
		{
			newPitch.Pitch += 1;
			CameraPitch += 1;
		}

		newPitch.Roll = 0;
		//if (mouseInput.X == 0)
		//	newPitch.Yaw = oCameraPitch;
		//newPitch.Pitch = newPitch.Pitch - 20;

		ArrowAngle = (ServerVelocity.X / maxVelocityX * 85) - 50;

		springArm->SetWorldRotation(newPitch);
		
		if(CurrentVelocity.X != 0)
		AuxImpact = CurrentVelocity.X;
}

void ABattleBumperPlayer::Reset()
{
	SetActorLocation(RespawnPosition);
	Lives = 3;
	CurrentDamage = 0;
	CurrentVelocity.X = 0;
	ServerVelocity.X = 0;
	respawning = true;
	GetWorld()->GetTimerManager().SetTimer(respawningTime, this, &ABattleBumperPlayer::Respawn, 4.0f, false);
}


float ABattleBumperPlayer::ReturnVelocityRotation() {
	return ArrowAngle;
}
void ABattleBumperPlayer::CollisionFalse()
{
	WasHit = false;
	GetWorldTimerManager().ClearTimer(DelayTimer);
}

void ABattleBumperPlayer::Server_ReliableFunctionCallThatRunsOnServer_Implementation(ABattleBumperPlayer * a, FVector NewLocation,  FRotator NewRotation, float v, float d, bool handbrake, bool shield)
{
	if (Role == ROLE_Authority) {
		a->SetActorLocationAndRotation(NewLocation, NewRotation, true);
	}
	a->CurrentPosition = a->GetActorLocation();
	if (a == this)
	{
		a->CurrentVelocity.X = v;
		//a->ServerVelocity.X = v;
	}

	a->ShieldActivated = shield;
	a->CurrentDamage = d;
	a->onHandbrake = handbrake;
	
	Client_ReliableFunctionCallThatRunsOnOwningClientOnly(a, NewLocation, NewRotation, v, d, handbrake, shield);
}


bool ABattleBumperPlayer::Server_ReliableFunctionCallThatRunsOnServer_Validate(ABattleBumperPlayer* a, FVector NewLocation, FRotator NewRotation, float v, float d, bool handbrake, bool shield)
{
	return true;
}

bool ABattleBumperPlayer::Server_BumperCollision_Validate(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght,ABattleBumperPlayer* a)
{
	return true;
}


void ABattleBumperPlayer::Server_BumperCollision_Implementation(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght, ABattleBumperPlayer* a)
{
	a->CollsionVector = -NImpactNormal + NForwardVector;
	a->ImpactStrenght = NImpactStrenght;

	a->WasHit = true;
	a->AddDamage = true;
	a->GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ABattleBumperPlayer::CollisionFalse, 1.0f, false);
	a->Client_BumperCollision(NImpactNormal, NForwardVector, NImpactStrenght,this);
}


void ABattleBumperPlayer::BumperCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght)
{
	//CollsionVector = -NImpactNormal + NForwardVector;
	//ImpactStrenght = NImpactStrenght;
	//WasHit = true;
	//AddDamage = true;
	//GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ABattleBumperPlayer::CollisionFalse, 1.0f, false);
	//if(Role==ROLE_Authority)
	Server_BumperCollision(NImpactNormal, NForwardVector, NImpactStrenght,this);
	
}
void ABattleBumperPlayer::Client_ReliableFunctionCallThatRunsOnOwningClientOnly_Implementation(ABattleBumperPlayer* a, FVector NewLocation, FRotator NewRotation, float v, float d, bool handbrake, bool shield)
{
	//if (Role == ROLE_AutonomousProxy) {
	ABattleBumperPlayer* player = nullptr;
	FVector L;
	FRotator R;
	if (Role == ROLE_Authority) {
	//	a->SetActorLocationAndRotation(NewLocation, NewRotation, true);


	}
	if (Role == ROLE_AutonomousProxy) {

		//if (a->SNewLocation != NewLocation || a->SNewRotation != NewRotation) {
		//	return;
		//}
		//else
		//{
		//a->SetActorLocationAndRotation(a->CurrentPosition, R, true);
		//}
		if (this != a) {
			a->SetActorLocationAndRotation(NewLocation, NewRotation, true);
			//a->ShieldActivated = shield;
		}

	}
	else {
		a->SetActorLocationAndRotation(NewLocation, NewRotation, true);
		a->ShieldActivated = shield;
	}
	//a->CurrentPosition = a->GetActorLocation();
	if (a == this)
	{
		//a->ShieldActivated = shield;
		//a->CurrentVelocity.X = v;
		a->ServerVelocity.X = v;
	}
	
	a->CurrentDamage = d;
	a->onHandbrake = handbrake;
	


	//}
	//for (TActorIterator<APawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
	//	APawn* p = *ActorItr;
	//	ABattleBumperPlayer* bumper = Cast<ABattleBumperPlayer>(p);
	//	if (bumper == a && !a->IsLocallyControlled()) {
	//		a->SetActorLocationAndRotation(NewLocation, NewRotation, true);
	//		a->CurrentPosition = a->GetActorLocation();
	//		a->ServerVelocity.X = v;
	//		a->CurrentDamage = d;
	//		a->onHandbrake = handbrake;
	//		a->ShieldActivated = shield;
	//	}
	//}
}

void ABattleBumperPlayer::Client_BumperCollision_Implementation(FVector NImpactNormal, FVector NForwardVector,
float NImpactStrenght, ABattleBumperPlayer* a)
{
	a->CollsionVector = -NImpactNormal + NForwardVector;
	a->ImpactStrenght = NImpactStrenght;
	a->WasHit = true;
	a->AddDamage = true;
	a->GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ABattleBumperPlayer::CollisionFalse, 1.0f, false);
}

void ABattleBumperPlayer::ClientTo_BumperCollision_Implementation(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght, ABattleBumperPlayer* a)
{
	a->CollsionVector = -NImpactNormal + NForwardVector;
	a->ImpactStrenght = NImpactStrenght;
	a->WasHit = true;
	a->AddDamage = true;
	a->GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ABattleBumperPlayer::CollisionFalse, 1.0f, false);
	a->ServerTo_BumperCollision(NImpactNormal, NForwardVector, NImpactStrenght, this);
}

// Called to bind functionality to input
void ABattleBumperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Respond when our "Grow" key is pressed or released.
	InputComponent->BindAction("Grow", IE_Pressed, this, &ABattleBumperPlayer::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &ABattleBumperPlayer::StopGrowing);

	InputComponent->BindAxis("MouseYawn", this, &ABattleBumperPlayer::mouseYawn);
	InputComponent->BindAxis("MousePitch", this, &ABattleBumperPlayer::mousePitch);

	// Respond every frame to the values of our two movement axes, "MoveForward" and "MoveRight".
	InputComponent->BindAxis("MoveForward", this, &ABattleBumperPlayer::Move_XAxis);
	InputComponent->BindAxis("MoveRight", this, &ABattleBumperPlayer::Move_YAxis);
	InputComponent->BindAction("Handbrake", IE_Pressed,  this, &ABattleBumperPlayer::Handbrake);
	InputComponent->BindAction("Handbrake", IE_Released, this, &ABattleBumperPlayer::ReleaseHandbrake);
	InputComponent->BindAction("Boost", IE_Pressed, this, &ABattleBumperPlayer::UseBoost);
	InputComponent->BindAction("Item", IE_Pressed, this, &ABattleBumperPlayer::ActivateItem);

}

void ABattleBumperPlayer::Handbrake() {
	HandbrakeForward = GetActorForwardVector();
	if (Grounded > 0) {
		onHandbrake = true;

		float direction = 1;
		if (CurrentRotation.Yaw < 0)
			direction *= -1;
		if (CurrentAcceleration.X != 0)
			CurrentRotation.Yaw += FMath::Clamp(direction, -1.0f, 1.0f) * maxVelocityY / 50;
	}
}

void ABattleBumperPlayer::UseBoost() {
	if (boost > 0) {
		CurrentVelocity.X += 500;
		boost--;
		if (CurrentVelocity.X > maxVelocityX)
			boosted = true;
	}
}

void ABattleBumperPlayer::DestroyShield()
{
	ShieldMesh->SetVisibility(false);
	ShieldActivated = false;
	GetWorldTimerManager().ClearTimer(ShieldTime);

}

void ABattleBumperPlayer::ActivateItem()
{
	
	if(ShieldCollection==true)
	{
		ActivateShield();
		ShieldCollection = false;
	}

	
}

void ABattleBumperPlayer::ShieldTimer()
{
	ShieldCollision = false;
	GetWorldTimerManager().ClearTimer(ShieldCollisionTimer);
}

void ABattleBumperPlayer::ReleaseHandbrake() {
	onHandbrake = false;
	if (CurrentRotation.Yaw < -maxVelocityY) {
		CurrentRotation.Yaw = -maxVelocityY;
	}
	else if(CurrentRotation.Yaw > maxVelocityY)
		CurrentRotation.Yaw = maxVelocityY;

	if (HandbrakeNormal < 0.3 && HandbrakeNormal != 0 && Grounded > 0) {
		CurrentVelocity.X += HandbrakeBoost;
	}
	if (CurrentVelocity.X > maxVelocityX)
		boosted = true;
}


void ABattleBumperPlayer::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward


	if(!boosted)
	CurrentAcceleration.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * maxAccelaration;


	if (CurrentAcceleration.X < 0 && maxAccelaration < oMaxAccelaration * 2)
		maxAccelaration = oMaxAccelaration * 2;
	if (maxAccelaration > oMaxAccelaration && CurrentAcceleration.X > 0)
			maxAccelaration = oMaxAccelaration;
	
	if (CurrentAcceleration.X > maxAccelaration) {
		CurrentAcceleration.X = maxAccelaration;
	}
	if (CurrentAcceleration.X < -maxAccelaration) {
		CurrentAcceleration.X = -maxAccelaration;
	}
}

void ABattleBumperPlayer::Move_YAxis(float AxisValue)
{
	//AddControllerYawInput(value * RotationBase);
	// Move at 100 units per second right or left
	CurrentAcceleration.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * maxVelocityY;
	if (CurrentAcceleration.Y > maxVelocityY) {
		CurrentAcceleration.Y = maxVelocityY;
	}
	if (CurrentAcceleration.Y < -maxVelocityY) {
		CurrentAcceleration.Y = -maxVelocityY;
	}
}

void ABattleBumperPlayer::mouseYawn(float axis)
{
	mouseInput.X = axis;
}

void ABattleBumperPlayer::mousePitch(float axis)
{
	mouseInput.Y = axis;
}

void ABattleBumperPlayer::StartGrowing()
{
	bGrowing = true;
}

void ABattleBumperPlayer::StopGrowing()
{
	bGrowing = false;
}
void ABattleBumperPlayer::ActivateShield()
{
	ShieldActivated = true;
}

void ABattleBumperPlayer::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//collision = true;
		//collision = true; 
	if (OtherActor && (OtherActor != this) && OtherComp)
	{

		AItemRandomizer* Item = Cast<AItemRandomizer>(OtherActor);
		bool blah = Item->PlayerCollided;
		if (Item && (Item->PlayerCollided == false)) {
			
			ShieldCollection = true;
			Item->PlayerCollided = true;
			Item->TimerFunc();
		}
		
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			if (collision == false)
			{
				//WasHit = false;
				CollisionTreshold = GetActorLocation();
				WallPosition = actor->GetActorLocation();

				//WorldCollision(SweepResult.ImpactNormal, GetActorForwardVector(), -1);
				//collision = true;
			}
		}

		ABoostActor* boostActor = Cast<ABoostActor>(OtherActor);

		if (boostActor) {
			if (boostActor->active && boost < 3) {
				boost++;
				boostActor->OnCollided();
			}
		}

		
		
		ABattleBumperPlayer* CollidedActors = Cast<ABattleBumperPlayer>(OtherActor);
		if (CollidedActors && CollidedActors->ShieldActivated == false && CurrentVelocity.X != 0)
		{
			float v = ServerVelocity.X;
			if (Role == ROLE_Authority)
			CollidedActors->Server_BumperCollision(SweepResult.ImpactNormal, GetActorForwardVector(), AuxImpact, CollidedActors);
			CollidedActors->playerAssasin = this;	
		}
	}


}



void ABattleBumperPlayer::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
	
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			collision = false;
			timeCollied = 0;
			//CollisionTreshold = FVector::ZeroVector;
		}
		CollidedActor2 = OtherActor;
		if (CollidedActor2->GetName() == "Wall") {
			collision = false;
		}
	}
}

void ABattleBumperPlayer::OnOverlapBegin2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AItemRandomizer* Item = Cast<AItemRandomizer>(OtherActor);
		bool blah = Item->PlayerCollided;
		if (Item && (Item->PlayerCollided == false)) {
			ShieldCollection = true;
			Item->PlayerCollided = true;
			Item->TimerFunc();
		}
		
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			if (collision == false)
			{
				// WasHit = false;
				CollisionTreshold = GetActorLocation();
				WallPosition = actor->GetActorLocation();
				//WorldCollision(SweepResult.ImpactNormal, GetActorForwardVector(), 1);
			

			}
		}

		ABoostActor* boostActor = Cast<ABoostActor>(OtherActor);

		if (boostActor) {
			if (boostActor->active && boost < 3) {
				boost++;
				boostActor->OnCollided();
			}
		}




		ABattleBumperPlayer* CollidedActors = Cast<ABattleBumperPlayer>(OtherActor);
		if (CollidedActors && CollidedActors->ShieldActivated == false && CurrentVelocity.X != 0)
		{
			float v = ServerVelocity.X;
			if (Role == ROLE_Authority)
			CollidedActors->Server_BumperCollision(SweepResult.ImpactNormal, GetActorForwardVector(), AuxImpact,CollidedActors);
			CollidedActors->playerAssasin = this;

			
		}
		CollidedActor2 = OtherActor;

		
	}
}

void ABattleBumperPlayer::OnOverlapEnd2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			collision = false;
			timeCollied = 0;

			//CollisionTreshold = FVector::ZeroVector;

		}
		CollidedActor2 = OtherActor;	
		if (CollidedActor2->GetName() == "Wall") {
			collision = false;
		}
	}
}

void ABattleBumperPlayer::OnOverlapBegin3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AItemRandomizer* Item = Cast<AItemRandomizer>(OtherActor);
		bool blah = Item->PlayerCollided;
		if (Item && (Item->PlayerCollided == false)) {
			ShieldCollection = true;
			Item->PlayerCollided = true;
			Item->TimerFunc();
		}
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			if (collision == false)
			{
				//WasHit = false;
				WallPosition = actor->GetActorLocation();

				CollisionTreshold = GetActorLocation();
				//WorldCollision(SweepResult.ImpactNormal, GetActorRightVector(), -1);
				//collision = true;
			}
		}
		ABoostActor* boostActor = Cast<ABoostActor>(OtherActor);

		if (boostActor) {
			if (boostActor->active && boost < 3) {
				boost++;
				boostActor->OnCollided();
			}
		}




		ABattleBumperPlayer* CollidedActors = Cast<ABattleBumperPlayer>(OtherActor);
		if (CollidedActors && CollidedActors->ShieldActivated == false  && CurrentVelocity.X != 0)
		{
			float v = ServerVelocity.X;
			if (Role == ROLE_Authority) {
				CollidedActors->Server_BumperCollision(SweepResult.ImpactNormal, GetActorForwardVector(), AuxImpact, CollidedActors);
				CollidedActors->playerAssasin = this;
			}
		
			
		}
		CollidedActor2 = OtherActor;
		//if (CollidedActor->Tags.Num() > 0) {
	
	}
}

void ABattleBumperPlayer::OnOverlapEnd3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{

		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			collision = false;
			timeCollied = 0;

			//CollisionTreshold = FVector::ZeroVector;

		}
		CollidedActor2 = OtherActor;

		if (CollidedActor2->GetName() == "Wall") {
			collisionleft = false;
		}
	}
}

void ABattleBumperPlayer::OnOverlapBegin4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AItemRandomizer* Item = Cast<AItemRandomizer>(OtherActor);
		bool blah = Item->PlayerCollided;
		if (Item && (Item->PlayerCollided == false)) {
			ShieldCollection = true;
			Item->PlayerCollided = true;
			Item->TimerFunc();
		}
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			if (collision == false)
			{
				//WasHit = false;
				WallPosition = actor->GetActorLocation();

				CollisionTreshold = GetActorLocation();
				//WorldCollision(SweepResult.ImpactNormal, GetActorRightVector(), 1);
				//collision = true;
			}
		}
		ABoostActor* boostActor = Cast<ABoostActor>(OtherActor);

		if (boostActor) {
			if (boostActor->active && boost < 3) {
				boost++;
				boostActor->OnCollided();
			}
		}



		ABattleBumperPlayer* CollidedActors = Cast<ABattleBumperPlayer>(OtherActor);
		if (CollidedActors && CollidedActors->ShieldActivated == false && CurrentVelocity.X != 0)
		{
			float v = CurrentVelocity.X;
			if(Role==ROLE_Authority)
			CollidedActors->Server_BumperCollision(SweepResult.ImpactNormal, GetActorForwardVector(), AuxImpact,CollidedActors);
			CollidedActors->playerAssasin = this;
		
			
		}
		CollidedActor2 = OtherActor;
		//if (CollidedActor->Tags.Num() > 0) {
		
	}
}

float ABattleBumperPlayer::ReturnVelocity() {
	return CurrentVelocity.X;
}

float ABattleBumperPlayer::ReturnDamage() {
	return CurrentDamage;
}

void ABattleBumperPlayer::WorldCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght)
{
	CollsionVectorWorld = NImpactNormal * NImpactStrenght;
	impactVector = NForwardVector * NImpactStrenght;
	//ImpactStrenght = NImpactStrenght;
	HitWorld = true;
	GetWorld()->GetTimerManager().SetTimer(DelayTimerWorld, this, &ABattleBumperPlayer::CollisionWorldFalse, 1.0f, false);
}

void ABattleBumperPlayer::CollisionWorldFalse()
{
	HitWorld = false;
	GetWorldTimerManager().ClearTimer(DelayTimerWorld);
}

void ABattleBumperPlayer::BeginShieldTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ShieldTime, this, &ABattleBumperPlayer::DestroyShield, 2.0f, false);
	//ShieldActivated = false;
}

void ABattleBumperPlayer::OnOverlapEnd4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor2 = OtherActor;
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			collision = false;
			timeCollied = 0;

			//CollisionTreshold = FVector::ZeroVector;

		}
		if (CollidedActor2->GetName() == "Wall") {
			collisionright = false;
		}

	}
}

void ABattleBumperPlayer::OnOverlapBeginGround(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AGroundActor * actor = Cast<AGroundActor>(OtherActor);
		if (actor) {
			Grounded++;
			if (actor->type == 1)
				OnGround = true;

			GroundPosition = OverlappedComp->GetComponentLocation();
				//CurrentRotation.Roll = actor->GetActorRotation().Roll;		

				FVector Rotation = FVector(actor->GetActorRotation().Roll , 0, actor->GetActorRotation().Pitch);
				FVector CurrentRotation = GetActorRotation().Vector();
				FVector v = FVector::ZeroVector;
				if (Rotation.X < 0) {
					v = actor->GetActorRightVector();
					Rotation.X *= -1;
				}
				else if (Rotation.X > 0) {
					v = actor->GetActorRightVector() * -1;
				}
				if (Rotation.Z < 0) {
					v = actor->GetActorForwardVector().BackwardVector;
					Rotation.Z *= -1;
				}
				else if (Rotation.Z > 0) {
					v = actor->GetActorForwardVector();
				}
				GroundedForward = v;
				if (Rotation.X != 0.0f) {
					GroundedRotationValue = Rotation.X;
					CalculateSlopeRotation();
				}
				else if (Rotation.Z != 0.0f) {
					GroundedRotationValue = Rotation.Z;
					CalculateSlopeRotation();
				}
				if(Rotation.Z == 0 && Rotation.X == 0 ) {
					GroundedRotationValue = 0;
					GroundRotation.Pitch = 0;
				}
		}
		ADeathZoneActor * deathactor = Cast<ADeathZoneActor>(OtherActor);

		if (deathactor) {
			SetActorLocation(RespawnPosition);
			respawning = true;
			CurrentVelocity.X = 0;
			ServerVelocity.X = 0;
			Lives -= 1;
			if (Lives == 0) {
				//if (controller) {
				//	gameInstance->RemovePlayer(this);
				//	controller->DisableInput(controller);
				//	if (playerAssasin) {
				//		controller->Possess(playerAssasin);
				//	}
				//	else
				//		controller->Possess(gameInstance->GetRandomPlayer());
				//}
			}
			else
			GetWorld()->GetTimerManager().SetTimer(respawningTime, this, &ABattleBumperPlayer::Respawn,	4.0f, false);
		}
	}
}

void ABattleBumperPlayer::Respawn() {
	respawning = false;
}

void ABattleBumperPlayer::ShieldHit(FVector NImpactNormal)
{
	if(CurrentVelocity.X >= 0)
	ShieldVector = NImpactNormal*5 - GetActorForwardVector();
	else if(CurrentVelocity.X < 0)
	ShieldVector = NImpactNormal * 5 + GetActorForwardVector() * 10;
	if(ServerVelocity.X != 0)
	YourVelocityShield = ServerVelocity.X;
	if (CurrentVelocity.X != 0)
	YourVelocityShield = CurrentVelocity.X;
	CurrentVelocity.X = 0;
	ServerVelocity.X = 0;
	AddDamageShield = true;
	ShieldCollision = true;
	GetWorld()->GetTimerManager().SetTimer(ShieldCollisionTimer, this, &ABattleBumperPlayer::ShieldTimer, 1.0f, false);
}

void ABattleBumperPlayer::OnOverlapEndGround(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AGroundActor* actor = Cast<AGroundActor>(OtherActor);

		if (actor) {
			//GroundRotation = nullptr;
			if (actor->type == 1)
				OnGround = false;

			Grounded--;

			//if (GetActorRotation().Pitch > 0 && Grounded == 0) {
			//	GroundRotation.Pitch = -GetActorRotation().Pitch;
			//}
		}
	}
}

void ABattleBumperPlayer::OnOverlapBeginShield(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ShieldActivated == true)
	{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			ABattleBumperPlayer* CollidedActors = Cast<ABattleBumperPlayer>(OtherActor);
			if (CollidedActors)
			{
				//CollidedActors->CurrentVelocity.X = 0;
				WasHit = false;
				CollidedActors->CurrentAcceleration.X = 0;
				BeginShieldTimer();
				CollidedActors->ShieldHit(SweepResult.ImpactNormal);
				
				
				//ShieldActivated = false;
			}
		}
		//ShieldCollected = false;
	}
}

void ABattleBumperPlayer::OnOverlapEndShield(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		


		
	}
}

void ABattleBumperPlayer::CalculateSlopeRotation(){
	GroundedNormal = FVector::DotProduct(GroundedForward, GetActorForwardVector());
	GroundRotation.Pitch = GroundedRotationValue;
	currentDistanceZ = GetActorLocation().Z - GroundPosition.Z;
	if (DistanceZ == 0.0f) {
		DistanceZ = 70;
	}
	if(OnGround)
	locationZ = DistanceZ - currentDistanceZ;
	if ((FVector::DotProduct(GroundedForward, GetActorForwardVector())) != 0)
		GroundRotation.Pitch *= FVector::DotProduct(GroundedForward, GetActorForwardVector());
}

void ABattleBumperPlayer::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AWallActor* actor = Cast<AWallActor>(OtherActor);
	if (actor) {
			WallPosition = actor->GetActorLocation();
			CollisionTreshold = GetActorLocation();
			collision = true;
			WorldCollision(Hit.ImpactNormal*1.5f, GetActorForwardVector(), 1);
	}
	ABattleBumperPlayer* CollidedActors = Cast<ABattleBumperPlayer>(OtherActor);
	if (CollidedActors && CollidedActors != this && CollidedActors->ShieldActivated == false)
	{
		//if(Role==ROLE_Authority)
		//CollidedActors->Server_BumperCollision(Hit.ImpactNormal, GetActorForwardVector(), CurrentVelocity.X,this);
		
	}
}

void ABattleBumperPlayer::ServerTo_BumperCollision_Implementation(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght, ABattleBumperPlayer* a)
{
	a->CollsionVector = -NImpactNormal + NForwardVector;
	a->ImpactStrenght = NImpactStrenght;
	a->WasHit = true;
	a->AddDamage = true;
	a->GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ABattleBumperPlayer::CollisionFalse, 1.0f, false);
}

bool ABattleBumperPlayer::ServerTo_BumperCollision_Validate(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght, ABattleBumperPlayer* a)
{
	return true;
}
