// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "Camera/CameraComponent.h"
#include "SnakeBase.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
APlayerPawnBase::APlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
	//RootComponent = PawnCamera;
	SetRootComponent(PawnCamera);
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(-90, 0, 0));
	CreateSnakeActor();
}



// Called every frame
void APlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawnBase::Move);


	}
	

}

void APlayerPawnBase::CreateSnakeActor()
{
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeActorClass, FTransform());
}

void APlayerPawnBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (IsValid(SnakeActor))
		{
			if (MovementVector.Y > 0 && SnakeActor->LastMoveDirection!=EMovementDirection::DOWN)
			{
				SnakeActor->LastMoveDirection = EMovementDirection::UP;
			}
			else if(MovementVector.Y < 0 && SnakeActor->LastMoveDirection != EMovementDirection::UP)
			{
				SnakeActor->LastMoveDirection = EMovementDirection::DOWN;
			}
			else if (MovementVector.X > 0 && SnakeActor->LastMoveDirection != EMovementDirection::RIGHT)
			{
				SnakeActor->LastMoveDirection = EMovementDirection::LEFT;
			}
			else if (MovementVector.X < 0 && SnakeActor->LastMoveDirection != EMovementDirection::LEFT)
			{
				SnakeActor->LastMoveDirection = EMovementDirection::RIGHT;
			}
		}
	}
}