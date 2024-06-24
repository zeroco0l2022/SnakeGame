// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactible.h"
#include "SnakeBase.generated.h"

class ASnakeElementBase;



UENUM()

enum class EMovementDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFoodEatDelegate);



UCLASS()

class SNAKEGAME_API ASnakeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeBase();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeElementBase> SnakeElementClass;

	UPROPERTY(EditDefaultsOnly)
	float ElementSize;

	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed;

	UPROPERTY(BlueprintReadOnly)
	int Points = 0;


	UPROPERTY()
	TArray <ASnakeElementBase*> SnakeElements;


	UPROPERTY()
	EMovementDirection LastMoveDirection;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FFoodEatDelegate OnFoodEat;

	UPROPERTY()
	bool CanMove;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddSnakeElement(int ElementsNum = 1);

	void Move();

	UFUNCTION()
	void SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other);

	UFUNCTION()
	TArray <FVector> GetSnakeElementsLocation();
};
