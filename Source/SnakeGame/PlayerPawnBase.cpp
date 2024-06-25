// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "LevelObjects.h"
#include "Camera/CameraComponent.h"
#include "SnakeBase.h"
#include "Food.h"
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
	SetRootComponent(PawnCamera);
	PawnCamera->SetFieldOfView(88);
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(-90, 0, 0));
	CreateSnakeActor();
	CreateFloorAndWalls();
	SpawnFood();
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

void APlayerPawnBase::CreateFloorAndWalls()
{
	FVector CubeScale = FVector();
	double Side = SnakeActor->ElementSize * 0.2;
	CubeScale = FVector(Side, Side, 0.1);
	FTransform CubeSpawn;
	CubeSpawn.SetLocation(FVector(0, 0, -50));
	Cube = GetWorld()->SpawnActor<ALevelObjects>(CubeClass, CubeSpawn);
	Cube->SetActorScale3D(CubeScale);
	
}

void APlayerPawnBase::CreateSnakeActor()
{
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeActorClass, FTransform());
	if (SnakeActor) {
		SnakeActor->OnFoodEat.AddDynamic(this, &APlayerPawnBase::AddPoints);
	}
	
}

void APlayerPawnBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (IsValid(SnakeActor) && SnakeActor->CanMove)
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
			SnakeActor->CanMove = false;
		}
	}
}

void APlayerPawnBase::SpawnFood()
{
	FVector Min, Max, Orgin, BoxExtent;
	Cube->Cube->GetLocalBounds(Min, Max);
	Cube->GetActorBounds(false, Orgin, BoxExtent);
		FVector FoodLocation = FoodSpawnCoordinates(BoxExtent.X - 5, Max.X);
		FTransform FoodSpawn;
		FoodSpawn.SetLocation(FoodLocation);
		FoodSpawn.SetScale3D(FVector(0.3, 0.3, 0.3));
		FActorSpawnParameters SpawnInfo;
		Food = GetWorld()->SpawnActor<AFood>(FoodClass, FoodSpawn);
}

int APlayerPawnBase::GetRandomCoordinate(float Side, float Offset)
{
	
	float Delta = (2 * Side / Offset) ;
	Delta = FMath::FRandRange(2, Delta-2);
	int32 RoundedDelta = FMath::RoundToInt(Delta);
	if (RoundedDelta % 2 != 0) {
		RoundedDelta = abs(RoundedDelta - 1) * (RoundedDelta / abs(RoundedDelta));
	}
	int32 Coordinate = ((Side) - RoundedDelta * Offset);
	return Coordinate;
}

FVector APlayerPawnBase::FoodSpawnCoordinates(float Side, float Offset)
{
	bool bCanSpawn = true;
	FVector SpawnCoordinates = FVector(GetRandomCoordinate(Side, Offset), GetRandomCoordinate(Side, Offset), 0);
	TArray <FVector> SnakeElementsLocation = SnakeActor->GetSnakeElementsLocation();
	for (int i = SnakeElementsLocation.Num() - 1; i > 0; i--)
	{
		if (SnakeElementsLocation[i] == SpawnCoordinates)
		{
			SpawnCoordinates.Y = GetRandomCoordinate(Side, Offset);
			i = SnakeElementsLocation.Num() - 1;
		}
	}
	return SpawnCoordinates;
}

void APlayerPawnBase::AddPoints_Implementation()
{
	Points++;
	SpawnFood();
}


