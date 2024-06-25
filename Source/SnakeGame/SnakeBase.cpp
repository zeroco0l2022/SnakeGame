// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Interactible.h"
#include "Food.h"
#include "LevelObjects.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 10.f;
	MovementSpeed = 0.6f;
	LastMoveDirection = EMovementDirection::UP;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
	CanMove = true;
	
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	FVector NewLocation;
	for (int i = 0; i < ElementsNum; i++)
	{
		if (!SnakeElements.IsEmpty())
		{
			NewLocation = FVector(SnakeElements.Last()->GetActorLocation() - (SnakeElements.Last()->GetActorForwardVector() + SnakeElements.Last()->GetActorRightVector()).Normalize() * ElementSize);
		}
		else
		{
			NewLocation = GetActorLocation();
		}
		FTransform NewTransform = FTransform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElem->SnakeOwner = this;
		int32 ElementIndex = SnakeElements.Add(NewSnakeElem);
		if (ElementIndex == 0)
		{
			NewSnakeElem->SetFirstElementType();
		} 
	}
	
}

void ASnakeBase::Move()
{

	FVector MovementVector(FVector::ZeroVector);
	switch (LastMoveDirection)
	{
	case EMovementDirection::UP:
		MovementVector.X += ElementSize;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X -= ElementSize;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y += ElementSize;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y -= ElementSize;
		break;
	}
	AddActorWorldOffset(MovementVector);

	
	SnakeElements[0]->SetActorRotation(MovementVector.Rotation());
	SnakeElements[0]->ToggleCollision();
	CanMove = true;

	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i-1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other)
{
	if (IsValid(OverlappedElement))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverlappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;

		if (Other->IsA(AFood::StaticClass()))
		{
				AFood* Food = Cast<AFood>(Other);
				if (Food)
				{
					if (bIsFirst)
					{
						Food->Destroy();
						OnFoodEat.Broadcast();
						MovementSpeed = MovementSpeed * 0.98;
						SetActorTickInterval(MovementSpeed);
					}
				}
		}

		if (Other->IsA(ALevelObjects::StaticClass()))
		{
			Destroy();
		}
		
		IInteractible* InteractibleInterface = Cast<IInteractible>(Other);
		
		if (InteractibleInterface)
		{
			InteractibleInterface->Interact(this, bIsFirst);
		}
	}
}

TArray<FVector> ASnakeBase::GetSnakeElementsLocation()
{
	TArray<FVector> ElemLocation;
	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		ElemLocation.Add(SnakeElements[i]->GetActorLocation());
	}
	return ElemLocation;
}



