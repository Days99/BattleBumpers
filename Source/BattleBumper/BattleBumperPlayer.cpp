// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBumperPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"

// Sets default values
ABattleBumperPlayer::ABattleBumperPlayer()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create a dummy root component we can attach things to.
	
	// Create a camera and a visible object

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	RootComponent = mesh;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	springArm->AttachTo(RootComponent);
	springArm->TargetArmLength = 450.0f;
	
	springArm->SetWorldRotation(FRotator(-60.f,0.0f,0.0f));

	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	
	
}

// Called when the game starts or when spawned
void ABattleBumperPlayer::BeginPlay()
{
	Super::BeginPlay();
	
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


	float CurrentScale = OurVisibleComponent->GetComponentScale().X;
	if (bGrowing)
	{
		// Grow to double size over the course of one second
		CurrentScale += DeltaTime;
	}
	else
	{
		// Shrink half as fast as we grow
		CurrentScale -= (DeltaTime * 0.5f);
	}
	// Make sure we never drop below our starting size, or increase past double size.
	CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
	OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));



	if (CurrentVelocity.X > dragX && CurrentAcceleration.X == 0)
	{
		CurrentAcceleration.X -= dragX;
	}
	if (CurrentVelocity.X < -dragX && CurrentAcceleration.X == 0)
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
	if (CurrentAcceleration.X == 0) {
		CurrentVelocity.X = 0;
		maxVelocityY = 10;
	}
	else {
		maxVelocityY = 30;
		CurrentVelocity.X += CurrentAcceleration.X / 10;
	}

	if (CurrentAcceleration.Y == 0) {
		CurrentRotation.Add(0, -CurrentRotation.Yaw, 0);
	}
	else  
	CurrentRotation.Add(0, CurrentAcceleration.Y / 10,0);

	if (CurrentVelocity.X > maxVelocityX && CurrentAcceleration.X > 0) 
	{
		CurrentVelocity.X = maxVelocityX;
	}

	if (CurrentVelocity.X < maxVelocityRX && CurrentAcceleration.X < 0)
	{
		CurrentVelocity.X = maxVelocityRX;
	}
	
	if(CurrentRotation.Yaw > maxVelocityY && CurrentAcceleration.Y > 0)
	{
		CurrentRotation.Yaw = maxVelocityY;
	}
	
	if (CurrentRotation.Yaw < -maxVelocityY && CurrentAcceleration.Y < 0)
	{
		CurrentRotation.Yaw = -maxVelocityY;
	}



	// Handle movement based on our "MoveX" and "MoveY" axes

	if (!CurrentVelocity.IsZero() || !CurrentRotation.IsZero())
	{
		FRotator NewRotation = GetActorRotation() + (CurrentRotation * DeltaTime);
		FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * CurrentVelocity.X * DeltaTime);
		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
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
}

void ABattleBumperPlayer::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
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
