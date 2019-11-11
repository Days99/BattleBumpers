// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyOwner = GetOwner();

	// Create a dummy root component we can attach things to.

	// Create a camera and a visible object
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	


	bReplicates = true;

	

	//ServerMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("ServerMovement"));






	//ServerMovement->AddToRoot();
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));

	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	springArm->SetupAttachment(RootComponent);
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

void AMyCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyCharacter, MyOwner);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	FrontTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
	FrontTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd);


	BackTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin2);
	BackTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd2);

	LeftTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin3);
	LeftTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd3);

	RightTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin4);
	RightTriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd4);
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
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
		CurrentRotation.Add(0, CurrentAcceleration.Y / 1000, 0);

	if (CurrentVelocity.X > maxVelocityX && CurrentAcceleration.X > 0 && !boosted)
	{
		CurrentVelocity.X = maxVelocityX;
	}

	if (CurrentVelocity.X < maxVelocityRX && CurrentAcceleration.X < 0)
	{
		CurrentVelocity.X = maxVelocityRX;
	}

	if (CurrentRotation.Yaw > maxVelocityY && CurrentAcceleration.Y > 0 && !uHandbrake)
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
		//GetVelocity().Set(Cu)
		AddMovementInput(GetActorForwardVector(),CurrentVelocity.X / 1000);
		//if(NewRotation != GetActorRotation())
		//SetActorRotation(NewRotation);
	}
}

void AMyCharacter::MoveForward(float value)
{
	// Move at 100 units per second forward or backward
	if (!boosted)
		CurrentAcceleration.X = FMath::Clamp(value, -1.0f, 1.0f) * maxAccelaration;
	if (CurrentAcceleration.X > maxAccelaration) {
		CurrentAcceleration.X = maxAccelaration;
	}
	if (CurrentAcceleration.X < -maxAccelaration) {
		CurrentAcceleration.X = -maxAccelaration;
	}
}

void AMyCharacter::MoveRight(float value)
{
	AddControllerYawInput(value * RotationBase);
	// Move at 100 units per second right or left
	CurrentAcceleration.Y = FMath::Clamp(value, -1.0f, 1.0f) * maxVelocityY;
	if (CurrentAcceleration.Y > maxVelocityY) {
		CurrentAcceleration.Y = maxVelocityY;
	}
	if (CurrentAcceleration.Y < -maxVelocityY) {
		CurrentAcceleration.Y = -maxVelocityY;
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Respond when our "Grow" key is pressed or released.
	PlayerInputComponent->BindAction("Grow", IE_Pressed, this, &AMyCharacter::StartGrowing);
	PlayerInputComponent->BindAction("Grow", IE_Released, this, &AMyCharacter::StopGrowing);

	PlayerInputComponent->BindAxis("MouseYawn", this, &AMyCharacter::mouseYawn);
	PlayerInputComponent->BindAxis("MousePitch", this, &AMyCharacter::mousePitch);

	// Respond every frame to the values of our two movement axes, "MoveForward" and "MoveRight".
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AMyCharacter::Handbrake);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AMyCharacter::ReleaseHandbrake);
	PlayerInputComponent->BindAction("Boost", IE_Pressed, this, &AMyCharacter::UseBoost);

}

void AMyCharacter::Handbrake() {
	if (!uHandbrake) {
		HandbrakeForward = GetActorForwardVector();
		uHandbrake = true;
		RotationBase *= 2;
	}

	float direction = 1;
	if (CurrentRotation.Yaw < 0)
		direction *= -1;
	if (CurrentAcceleration.X != 0)
		CurrentRotation.Yaw += FMath::Clamp(direction, -1.0f, 1.0f) * maxVelocityY / 2;
}

void AMyCharacter::UseBoost() {
	if (boost > 0) {
		//CurrentVelocity.X += 500;
		//GetVelocity().SetComponentForAxis(EAxis::X, 1.5);
		boost--;
		if (CurrentVelocity.X > maxVelocityX)
			boosted = true;
	}
}

void AMyCharacter::ReleaseHandbrake() {
	RotationBase = 0.1f;
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


void AMyCharacter::mouseYawn(float axis)
{
	mouseInput.X = axis;
}

void AMyCharacter::mousePitch(float axis)
{
	mouseInput.Y = axis;
}

void AMyCharacter::StartGrowing()
{
	bGrowing = true;
}

void AMyCharacter::StopGrowing()
{
	bGrowing = false;
}

void AMyCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor = OtherActor;
		//if (CollidedActor->Tags.Num() > 0) {
		if (CollidedActor->GetActorLabel() == "Boost" && boost < 3) {
			boost++;
			OtherActor->Destroy();
		}

		if (CollidedActor->GetName() == "Wall") {
			collision = true;
		}

		//}
	}
}



void AMyCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor2 = OtherActor;
		if (CollidedActor2->GetName() == "Wall") {
			collision = false;
		}
	}
}

void AMyCharacter::OnOverlapBegin2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor = OtherActor;
		//if (CollidedActor->Tags.Num() > 0) {
		if (CollidedActor->GetActorLabel() == "Boost" && boost < 3) {
			boost++;
			OtherActor->Destroy();
		}

		if (CollidedActor->GetName() == "Wall") {
			collision = true;
		}
	}
}

void AMyCharacter::OnOverlapEnd2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor2 = OtherActor;

		if (CollidedActor2->GetName() == "Wall") {
			collision = true;
		}
	}
}

void AMyCharacter::OnOverlapBegin3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor = OtherActor;
		//if (CollidedActor->Tags.Num() > 0) {
		if (CollidedActor->GetActorLabel() == "Boost" && boost < 3) {
			boost++;
			OtherActor->Destroy();
		}

		if (CollidedActor->GetName() == "Wall") {
			collisionleft = true;
		}
	}
}

void AMyCharacter::OnOverlapEnd3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor2 = OtherActor;

		if (CollidedActor2->GetName() == "Wall") {
			collisionleft = false;
		}
	}
}

void AMyCharacter::OnOverlapBegin4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//collision = true;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor = OtherActor;
		//if (CollidedActor->Tags.Num() > 0) {
		if (CollidedActor->GetActorLabel() == "Boost" && boost < 3) {
			boost++;
			OtherActor->Destroy();
		}

		if (CollidedActor->GetName() == "Wall") {
			collisionright = true;
		}
	}
}

float AMyCharacter::ReturnVelocity() {
	return CurrentVelocity.X;
}

void AMyCharacter::OnOverlapEnd4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CollidedActor2 = OtherActor;

		if (CollidedActor2->GetName() == "Wall") {
			collisionright = false;
		}
	}
}

