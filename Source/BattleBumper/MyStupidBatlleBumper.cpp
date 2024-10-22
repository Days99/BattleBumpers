// Fill out your copyright notice in the Description page of Project Settings.

#include "MyStupidBatlleBumper.h"
#include "Components/PrimitiveComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Particles/ParticleSystem.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/Actor.h"

// Sets default values
AMyStupidBatlleBumper::AMyStupidBatlleBumper()
{
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyOwner = GetOwner();

	// Create a dummy root component we can attach things to.

	// Create a camera and a visible object
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//MovementCharacter = CreateDefaultSubobject<UCharacterMovementComponent>((TEXT("Movement")));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Assets/NewBattleBumper"));
	UStaticMesh* Asset = MeshAsset.Object;
	myMesh->SetStaticMesh(Asset);

	bReplicates = true;

	//OurCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("OurCollider"));

	//ServerMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("ServerMovement"));


	//OurCollider->SetSimulatePhysics(true);
	//OurCollider->SetEnableGravity(true);

	//OurCollider->SetNotifyRigidBodyCollision(true);

	//ootComponent = OurCollider;
	//myMesh->AttachTo(RootComponent);
	RootComponent = myMesh;
	//ServerMovement->AddToRoot();
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));

	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	springArm->AttachTo(RootComponent);
	springArm->TargetArmLength = 450.0f;

	springArm->SetWorldRotation(FRotator(-60.f, 0.0f, 0.0f));

	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);



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


// Called when the game starts or when spawned
void AMyStupidBatlleBumper::BeginPlay()
{
	Super::BeginPlay();

	FrontTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyStupidBatlleBumper::OnOverlapBegin);
	FrontTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMyStupidBatlleBumper::OnOverlapEnd);


	BackTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyStupidBatlleBumper::OnOverlapBegin2);
	BackTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMyStupidBatlleBumper::OnOverlapEnd2);

	LeftTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyStupidBatlleBumper::OnOverlapBegin3);
	LeftTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMyStupidBatlleBumper::OnOverlapEnd3);

	RightTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyStupidBatlleBumper::OnOverlapBegin4);
	RightTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMyStupidBatlleBumper::OnOverlapEnd4);
}

void AMyStupidBatlleBumper::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyStupidBatlleBumper, MyOwner);
}

// Called every frame
void AMyStupidBatlleBumper::Tick(float DeltaTime)
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
	if (CurrentRotation.Yaw > dragZ&& CurrentAcceleration.Y == 0)
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
		CurrentRotation.Add(0, CurrentAcceleration.Y / 10, 0);

	if (CurrentVelocity.X > maxVelocityX&& CurrentAcceleration.X > 0 && !boosted)
	{
		CurrentVelocity.X = maxVelocityX;
	}

	if (CurrentVelocity.X < maxVelocityRX && CurrentAcceleration.X < 0)
	{
		CurrentVelocity.X = maxVelocityRX;
	}

	if (CurrentRotation.Yaw > maxVelocityY&& CurrentAcceleration.Y > 0 && !uHandbrake)
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

	if (!CurrentVelocity.IsZero() || !CurrentRotation.IsZero())
	{

		FRotator NewRotation = GetActorRotation() + (CurrentRotation * DeltaTime);
		FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * CurrentVelocity.X * DeltaTime);
		if (uHandbrake && !CurrentVelocity.IsZero()) {
			HandbrakeNormal = FVector::DotProduct(GetActorForwardVector(), HandbrakeForward);
			if (HandbrakeNormal < 0.1) {
				HandbrakeNormal = 0.3;
			}
			NewLocation = NewLocation + (HandbrakeForward * (CurrentVelocity.X / 20)) / HandbrakeNormal * DeltaTime;
		}
		//SetActorLocationAndRotation(NewLocation, NewRotation);
		//MovementComponent->Add
	}
}

// Called to bind functionality to input
void AMyStupidBatlleBumper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Respond when our "Grow" key is pressed or released.
	InputComponent->BindAction("Grow", IE_Pressed, this, &AMyStupidBatlleBumper::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &AMyStupidBatlleBumper::StopGrowing);

	InputComponent->BindAxis("MouseYawn", this, &AMyStupidBatlleBumper::mouseYawn);
	InputComponent->BindAxis("MousePitch", this, &AMyStupidBatlleBumper::mousePitch);

	// Respond every frame to the values of our two movement axes, "MoveForward" and "MoveRight".
	InputComponent->BindAxis("MoveForward", this, &AMyStupidBatlleBumper::Move_XAxis);
	InputComponent->BindAxis("MoveRight", this, &AMyStupidBatlleBumper::Move_YAxis);
	InputComponent->BindAction("Handbrake", IE_Pressed, this, &AMyStupidBatlleBumper::Handbrake);
	InputComponent->BindAction("Handbrake", IE_Released, this, &AMyStupidBatlleBumper::ReleaseHandbrake);
	InputComponent->BindAction("Boost", IE_Pressed, this, &AMyStupidBatlleBumper::UseBoost);

}

void AMyStupidBatlleBumper::Handbrake() {
	if (!uHandbrake) {
		HandbrakeForward = GetActorForwardVector();
		uHandbrake = true;
	}

	float direction = 1;
	if (CurrentRotation.Yaw < 0)
		direction *= -1;
	if (CurrentAcceleration.X != 0)
		CurrentRotation.Yaw += FMath::Clamp(direction, -1.0f, 1.0f) * maxVelocityY / 2;
}

void AMyStupidBatlleBumper::UseBoost() {
	if (boost > 0) {
		CurrentVelocity.X += 500;
		boost--;
		if (CurrentVelocity.X > maxVelocityX)
			boosted = true;
	}
}

void AMyStupidBatlleBumper::ReleaseHandbrake() {
	uHandbrake = false;
	if (CurrentRotation.Yaw < -maxVelocityY) {
		CurrentRotation.Yaw = -maxVelocityY;
	}
	else if (CurrentRotation.Yaw > maxVelocityY)
		CurrentRotation.Yaw = maxVelocityY;

	if (HandbrakeNormal < 0.3) {
		CurrentVelocity.X += HandbrakeBoost;
	}
	if (CurrentVelocity.X > maxVelocityX)
		boosted = true;
}


void AMyStupidBatlleBumper::Move_XAxis(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
	// Move at 100 units per second forward or backward
	if (!boosted)
		CurrentAcceleration.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * maxAccelaration;
	if (CurrentAcceleration.X > maxAccelaration) {
		CurrentAcceleration.X = maxAccelaration;
	}
	if (CurrentAcceleration.X < -maxAccelaration) {
		CurrentAcceleration.X = -maxAccelaration;
	}
}

void AMyStupidBatlleBumper::Move_YAxis(float AxisValue)
{
	AddControllerYawInput(AxisValue);
	// Move at 100 units per second right or left
	CurrentAcceleration.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * maxVelocityY;
	if (CurrentAcceleration.Y > maxVelocityY) {
		CurrentAcceleration.Y = maxVelocityY;
	}
	if (CurrentAcceleration.Y < -maxVelocityY) {
		CurrentAcceleration.Y = -maxVelocityY;
	}
}

void AMyStupidBatlleBumper::mouseYawn(float axis)
{
	mouseInput.X = axis;
}

void AMyStupidBatlleBumper::mousePitch(float axis)
{
	mouseInput.Y = axis;
}

void AMyStupidBatlleBumper::StartGrowing()
{
	bGrowing = true;
}

void AMyStupidBatlleBumper::StopGrowing()
{
	bGrowing = false;
}

void AMyStupidBatlleBumper::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		

		//}
	}
}



void AMyStupidBatlleBumper::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
	
	}
}

void AMyStupidBatlleBumper::OnOverlapBegin2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
	
	}
}

void AMyStupidBatlleBumper::OnOverlapEnd2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		
	}
}

void AMyStupidBatlleBumper::OnOverlapBegin3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{

	}
}

void AMyStupidBatlleBumper::OnOverlapEnd3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{

	}
}

void AMyStupidBatlleBumper::OnOverlapBegin4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{

	}
}

float AMyStupidBatlleBumper::ReturnVelocity() {
	return CurrentVelocity.X;
}

void AMyStupidBatlleBumper::OnOverlapEnd4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		
	}
}

