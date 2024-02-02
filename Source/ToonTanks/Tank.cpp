// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	TankPlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		TankPlayerController->GetLocalPlayer()
	);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(InputMove, ETriggerEvent::Triggered, this, &ATank::Move);
	Input->BindAction(InputTurn, ETriggerEvent::Triggered, this, &ATank::Turn);
	Input->BindAction(InputFire, ETriggerEvent::Started, this, &ATank::Fire);
}

void ATank::Tick(float const DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		bool IsHit = TankPlayerController->GetHitResultUnderCursor(
			ECC_Visibility,
			false,
			HitResult
		);

		if (IsHit)
		{
			RotateTurret(HitResult.ImpactPoint);
		}
	}
}


void ATank::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();
	float MoveSpeed = MoveValue.X * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	FVector DeltaLocation = FVector(MoveSpeed, 0.f, 0.f);

	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(const FInputActionValue& Value)
{
	const FVector2d TurnValue = Value.Get<FVector2D>();
	const float TurnSpeed = TurnValue.X * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = TurnSpeed;

	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	UE_LOG(LogTemp, Warning, TEXT("Tank died"));
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}