// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBumperPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Particles/ParticleSystem.h"

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
	
	//mesh->SetEnableGravity(true);
	
	RootComponent = mesh;
	OurCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("OurCollider"));
	OurCollider->AttachTo(RootComponent);
	//OurCollider->SetSimulatePhysics(true);
	//OurCollider->SetEnableGravity(true);

	OurCollider->SetNotifyRigidBodyCollision(true);
	OurCollider->SetNotifyRigidBodyCollision(true);
	OurCollider->OnComponentHit.AddDynamic(this, &ABattleBumperPlayer::OnCompHit);
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	
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




	if (CurrentVelocity.X >= dragX && (CurrentAcceleration.X == 0 || boosted))
	{
		CurrentAcceleration.X -= dragX;
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

	if (!CurrentVelocity.IsZero() || !CurrentRotation.IsZero())
	{
		FRotator NewRotation = GetActorRotation() + (CurrentRotation * DeltaTime);
		FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * CurrentVelocity.X * DeltaTime);
		if (uHandbrake && !CurrentVelocity.IsZero()) {
			HandbrakeNormal = FVector::DotProduct(GetActorForwardVector(), HandbrakeForward);
			if (HandbrakeNormal < 0.1) {
				HandbrakeNormal = 0.3;
			}
			NewLocation = NewLocation + (HandbrakeForward * (CurrentVelocity.X / 5)) / HandbrakeNormal * DeltaTime;
		}
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
	InputComponent->BindAction("Handbrake", IE_Pressed,  this, &ABattleBumperPlayer::Handbrake);
	InputComponent->BindAction("Handbrake", IE_Released, this, &ABattleBumperPlayer::ReleaseHandbrake);

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

void ABattleBumperPlayer::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		
		if ( OtherActor->ActorHasTag(TEXT("Wall")) )
		{ 
			CurrentVelocity.X = 0;
			CurrentAcceleration.X = 0;
		}
	}
}
