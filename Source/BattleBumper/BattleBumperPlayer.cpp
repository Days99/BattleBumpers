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
#include "math.h"
#include "GroundActor.h"
#include "TimerManager.h"
#include "WallActor.h"


// Sets default values
ABattleBumperPlayer::ABattleBumperPlayer()
{
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyOwner = GetOwner();

	// Create a dummy root component we can attach things to.
	
	// Create a camera and a visible object
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	MovementCharacter = CreateDefaultSubobject<UCharacterMovementComponent>((TEXT("Movement")));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Assets/NewBattleBumper"));
	UStaticMesh * Asset = MeshAsset.Object;
	myMesh->SetStaticMesh(Asset);

	bReplicates = true;
	SetReplicateMovement(true);
	OurCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("OurCollider"));
	
	//ServerMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("ServerMovement"));

	
	//OurCollider->SetSimulatePhysics(true);
	//OurCollider->SetEnableGravity(true);

	OurCollider->SetNotifyRigidBodyCollision(true);

	RootComponent = OurCollider;
	myMesh->AttachTo(RootComponent);

	//ServerMovement->AddToRoot();
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	springArm->AttachTo(RootComponent);
	springArm->TargetArmLength = 450.0f;
	
	springArm->SetWorldRotation(FRotator(-60.f,0.0f,0.0f));

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
	BackTriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	BackTriggerCapsule->SetCollisionProfileName(TEXT("Trigger 2"));
	BackTriggerCapsule->SetupAttachment(RootComponent);

	// declare trigger capsule
	LeftTriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule 3"));
	LeftTriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	LeftTriggerCapsule->SetCollisionProfileName(TEXT("Trigger 3"));
	LeftTriggerCapsule->SetupAttachment(RootComponent);

	// declare trigger capsule
	RightTriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule 4"));
	RightTriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	RightTriggerCapsule->SetCollisionProfileName(TEXT("Trigger 4"));
	RightTriggerCapsule->SetupAttachment(RootComponent);

}

void ABattleBumperPlayer::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABattleBumperPlayer, MyOwner);
}

// Called when the game starts or when spawned
void ABattleBumperPlayer::BeginPlay()
{
	Super::BeginPlay();

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

}

// Called every frame
void ABattleBumperPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle growing and shrinking based on our "Grow" action

	FRotator newYawn = GetActorRotation();
	
	newYawn.Yaw += mouseInput.X;
	

	FRotator newPitch = springArm->GetComponentRotation();
	newPitch.Yaw += mouseInput.X;
	newPitch.Pitch += mouseInput.Y;
	springArm->SetWorldRotation(newPitch);


	if (collision == true)
	{
		CurrentVelocity.X = 0;
		CurrentAcceleration.X = 0;
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

	if (CurrentVelocity.X >= dragX && CurrentAcceleration.X == 0)
	{
		CurrentAcceleration.X -= dragX;
	}
	if (CurrentVelocity.X >= dragXBoost && boosted) {
		CurrentAcceleration.X -= dragXBoost;
	}
	if (CurrentVelocity.X <= -dragX && CurrentAcceleration.X == 0)
	{
		CurrentAcceleration.X += dragX;
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
		maxVelocityY = 10;
	}
	else {
		maxVelocityY = 30;
	}
	if (!uHandbrake)
		CurrentVelocity.X += CurrentAcceleration.X;
	
	if (uHandbrake && CurrentVelocity.X > 0) {
		if (CurrentVelocity.X < HandbrakeAccelaration) {
			CurrentVelocity.X = 0;
		}
		else
		CurrentVelocity.X += HandbrakeAccelaration;
	}
	else if (uHandbrake && CurrentVelocity.X < 0) {
		if (CurrentVelocity.X > HandbrakeAccelaration) {
			CurrentVelocity.X = 0;
		}
		else
		CurrentVelocity.X -= HandbrakeAccelaration;
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
	
	if(CurrentRotation.Yaw > maxVelocityY && CurrentAcceleration.Y > 0 && !uHandbrake)
	{
		CurrentRotation.Yaw = maxVelocityY;
	}
	else if (uHandbrake && CurrentRotation.Yaw > maxVelocityY * 4) {
		CurrentRotation.Yaw = maxVelocityY * 4;
	}
	
	if (CurrentRotation.Yaw < -maxVelocityY && CurrentAcceleration.Y < 0 && !uHandbrake)
	{
		CurrentRotation.Yaw = -maxVelocityY;
	}
	else if (uHandbrake && CurrentRotation.Yaw < -maxVelocityY * 4) {
		CurrentRotation.Yaw = -maxVelocityY * 4;
	}

	if (boosted && CurrentVelocity.X < maxVelocityX) {
		boosted = false;
	}




	
	// Handle movement based on our "MoveX" and "MoveY" axes

	if (!CurrentVelocity.IsZero() || !CurrentRotation.IsZero() || Grounded >= 0)
	{

		FRotator NewRotation = GetActorRotation() + (CurrentRotation * DeltaTime);
		if (Grounded > 0 ) {
			CalculateSlopeRotation();
			if (NewRotation.Pitch < GroundRotation.Pitch) {
				NewRotation.Pitch += (40 + (5 * GroundRotation.Pitch * CurrentVelocity.X / maxVelocityX))* DeltaTime;
			}
			else if (NewRotation.Pitch > GroundRotation.Pitch) {
				NewRotation.Pitch -= (40 + (5 * GroundRotation.Pitch * CurrentVelocity.X / maxVelocityX)) * DeltaTime;
			}
			if (GroundRotation.Pitch == 0 && NewRotation.Pitch != 0) {
				NewRotation.Pitch = 0;
			}
		}
		else if (NewRotation.Pitch > -20 && Grounded == 0)
			NewRotation.Pitch -= (10 + (5 * ServerVelocity.X / maxVelocityX)) * DeltaTime;

		

		FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * ServerVelocity.X * DeltaTime);
		if (uHandbrake && !CurrentVelocity.IsZero()) {
			HandbrakeNormal = FVector::DotProduct(GetActorForwardVector(), HandbrakeForward);
			if (HandbrakeNormal < 0.1) {
				HandbrakeNormal = 0.3;
			}
			NewLocation = NewLocation + (HandbrakeForward * (ServerVelocity.X / 40)) / HandbrakeNormal * DeltaTime;
		}
		if (Grounded <= 0) {
			NewLocation = NewLocation + (CollsionVector*ImpactStrenght + (GetActorUpVector() * -350 ))* DeltaTime;
		}
		
		if (WasHit)
		{
			if(ImpactStrenght >= 800)
			NewLocation += (CollsionVector * ImpactStrenght + CollsionVector * CurrentDamage*5 + GetActorUpVector() * ImpactStrenght) * DeltaTime;
			else if (ImpactStrenght < 800)
			NewLocation += (CollsionVector * ImpactStrenght*2 + CollsionVector * CurrentDamage*5) * DeltaTime;
		}
		if (AddDamage)
		{
			CurrentDamage += ImpactStrenght*DeltaTime;;
			AddDamage = false;
		}
		
		if (HitWorld)
		{
			NewLocation += (CollsionVector * ImpactStrenght * 2) * DeltaTime;
		}
		//SetActorLocationAndRotation(NewLocation, NewRotation);
		Server_ReliableFunctionCallThatRunsOnServer(this, NewLocation, NewRotation, CurrentVelocity.X);

	}
}
void ABattleBumperPlayer::CollisionFalse()
{
	WasHit = false;
	GetWorldTimerManager().ClearTimer(DelayTimer);
}

void ABattleBumperPlayer::CollisionWorldFalse()
{
	HitWorld = false;
	GetWorldTimerManager().ClearTimer(DelayTimerWorld);
}

void ABattleBumperPlayer::Server_ReliableFunctionCallThatRunsOnServer_Implementation(ABattleBumperPlayer * a, FVector NewLocation, FRotator NewRotation, float v)
{
	if(Role == ROLE_Authority)
	Client_ReliableFunctionCallThatRunsOnOwningClientOnly(a, NewLocation, NewRotation, v);
}

bool ABattleBumperPlayer::Server_ReliableFunctionCallThatRunsOnServer_Validate(ABattleBumperPlayer* a, FVector NewLocation, FRotator NewRotation, float v)
{
	return true;
}
void ABattleBumperPlayer::Server_BumperCollision_Implementation(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght)
{
	if (Role == ROLE_Authority)
	{
		CollsionVector = -NImpactNormal + NForwardVector;
		ImpactStrenght = NImpactStrenght;
		
		WasHit = true;
		AddDamage = true;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ABattleBumperPlayer::CollisionFalse, 1.0f, false);
		
	}
}

bool ABattleBumperPlayer::Server_BumperCollision_Validate(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght)
{
	return true;
}

void ABattleBumperPlayer::BumperCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght)
{
	
	if (Role == ROLE_Authority)
	{
		Server_BumperCollision(NImpactNormal, NForwardVector, NImpactStrenght);
	}
	
}

void ABattleBumperPlayer::WorldCollision(FVector NImpactNormal, FVector NForwardVector, float NImpactStrenght)
{
	CollsionVector = NImpactNormal - NForwardVector;
	ImpactStrenght = NImpactStrenght;
	HitWorld = true;
	GetWorld()->GetTimerManager().SetTimer(DelayTimerWorld, this, &ABattleBumperPlayer::CollisionWorldFalse, 1.0f, false);
	
}




void ABattleBumperPlayer::Client_ReliableFunctionCallThatRunsOnOwningClientOnly_Implementation(ABattleBumperPlayer* a, FVector NewLocation, FRotator NewRotation, float v)
{
	a->SetActorLocationAndRotation(NewLocation, NewRotation);
	a->ServerVelocity.X = v;
	
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

}

void ABattleBumperPlayer::Handbrake() {
	if (!uHandbrake) {
		HandbrakeForward = GetActorForwardVector();
		uHandbrake = true;
	}

	float direction = 1;
	if (CurrentRotation.Yaw < 0)
		direction *= -1;
	if(CurrentAcceleration.X != 0)
	CurrentRotation.Yaw += FMath::Clamp(direction, -1.0f, 1.0f) * maxVelocityY / 2;
}

void ABattleBumperPlayer::UseBoost() {
	if (boost > 0) {
		CurrentVelocity.X += 500;
		boost--;
		if (CurrentVelocity.X > maxVelocityX)
			boosted = true;
	}
}

void ABattleBumperPlayer::ReleaseHandbrake() {
	uHandbrake = false;
	if (CurrentRotation.Yaw < -maxVelocityY) {
		CurrentRotation.Yaw = -maxVelocityY;
	}
	else if(CurrentRotation.Yaw > maxVelocityY)
		CurrentRotation.Yaw = maxVelocityY;

	if (HandbrakeNormal < 0.3) {
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

void ABattleBumperPlayer::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//collision = true;
		//collision = true; 
	
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			collision = true;
			WorldCollision(SweepResult.ImpactNormal, GetActorForwardVector(), 100);
		}
		
		ABattleBumperPlayer* CollidedActors = Cast<ABattleBumperPlayer>(OtherActor);
		if (CollidedActors)
		{
			float v = ServerVelocity.X;
			CollidedActors->BumperCollision(SweepResult.ImpactNormal, GetActorForwardVector(), v);
		}
	}


}



void ABattleBumperPlayer::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor2 = OtherActor;
		if (CollidedActor2->GetName() == "Wall") {
			collision = false;
		}
		AGroundActor* ground = Cast<AGroundActor>(OtherActor);
		if (ground) {
			collision = false;
		}
	}
}

void ABattleBumperPlayer::OnOverlapBegin2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			collision = true;
				WorldCollision(SweepResult.ImpactNormal, GetActorForwardVector(), -100);
		}
		
		CollidedActor2 = OtherActor;

		AGroundActor* ground = Cast<AGroundActor>(OtherActor);
		if (ground) {
			collision = true;
		}

		if (CollidedActor2->GetActorLabel() == "Boost" && boost < 3) {
			boost++;
			OtherActor->Destroy();
		}



		if (CollidedActor2->GetName() == "Wall") {
			collision = true;
		}
	}
}

void ABattleBumperPlayer::OnOverlapEnd2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{

		
		CollidedActor2 = OtherActor;	
		if (CollidedActor2->GetName() == "Wall") {
			collision = false;
		}
		AGroundActor* ground = Cast<AGroundActor>(OtherActor);
		if (ground) {
			collision = false;
		}
	}
}

void ABattleBumperPlayer::OnOverlapBegin3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			collision = true;
				WorldCollision(SweepResult.ImpactNormal, GetActorForwardVector(), 100);
		}
		CollidedActor2 = OtherActor;
		//if (CollidedActor->Tags.Num() > 0) {
		if (CollidedActor2->GetActorLabel() == "Boost" && boost < 3) {
			boost++;
			OtherActor->Destroy();
		}
		AGroundActor* ground = Cast<AGroundActor>(OtherActor);
		if (ground) {
			collisionleft = true;
		}
		if (CollidedActor2->GetName() == "Wall") {
			collisionleft = true;
		}
	}
}

void ABattleBumperPlayer::OnOverlapEnd3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor2 = OtherActor;

		if (CollidedActor2->GetName() == "Wall") {
			collisionleft = false;
		}
		AGroundActor* ground = Cast<AGroundActor>(OtherActor);
		if (ground) {
			collisionleft = false;
		}
	}
}

void ABattleBumperPlayer::OnOverlapBegin4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AWallActor* actor = Cast<AWallActor>(OtherActor);
		if (actor) {
			collision = true;
				WorldCollision(SweepResult.ImpactNormal, GetActorForwardVector(), 100);
		}
		CollidedActor2 = OtherActor;
		//if (CollidedActor->Tags.Num() > 0) {
		if (CollidedActor2->GetActorLabel() == "Boost" && boost < 3) {
			boost++;
			OtherActor->Destroy();
		}

		if (CollidedActor2->GetName() == "Wall") {
			collisionright = true;
		}
		AGroundActor* ground = Cast<AGroundActor>(OtherActor);
		if (ground) {
			collisionright = true;
		}
	}
}

float ABattleBumperPlayer::ReturnVelocity() {
	return CurrentVelocity.X;
}

float ABattleBumperPlayer::ReturnDamage() {
	return CurrentDamage;
}


void ABattleBumperPlayer::OnOverlapEnd4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor2 = OtherActor;

		if (CollidedActor2->GetName() == "Wall") {
			collisionright = false;
		}
		AGroundActor* ground = Cast<AGroundActor>(OtherActor);
		if (ground) {
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
				//CurrentRotation.Roll = actor->GetActorRotation().Roll;
				FVector Rotation = FVector(actor->GetActorRotation().Roll , 0, actor->GetActorRotation().Pitch);
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
				else if (Rotation.Z == 0 && Rotation.X == 0 && Grounded >= 1) {
					//GroundedRotationValue = 0;
					GroundRotation.Pitch = 0;
				}

		}
	}
}

void ABattleBumperPlayer::OnOverlapEndGround(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AGroundActor* actor = Cast<AGroundActor>(OtherActor);

		if (actor) {
			//GroundRotation = nullptr;
			Grounded--;
			if (GetActorRotation().Pitch > 0 && Grounded == 0) {
				GroundRotation.Pitch = -GetActorRotation().Pitch;
			}
		}
	}
}

void ABattleBumperPlayer::CalculateSlopeRotation(){
	GroundedNormal = FVector::DotProduct(GroundedForward, GetActorForwardVector());
	GroundRotation.Pitch = GroundedRotationValue * FVector::DotProduct(GroundedForward, GetActorForwardVector());
}

