// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawnBase.generated.h"


class UCameraComponent;
class ASnakeBase;
class UInputMappingContext;
class UInputAction;
class ALevelObjects;
class AFood;
struct FInputActionValue;

UCLASS()
class SNAKEGAME_API APlayerPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawnBase();

	UPROPERTY(BluePrintReadWrite)
	UCameraComponent* PawnCamera;


	UPROPERTY(BlueprintReadWrite)
	ASnakeBase* SnakeActor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeBase> SnakeActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(BluePrintReadWrite)
	AFood* Food;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFood>  FoodClass;

	UPROPERTY(BluePrintReadWrite)
	ALevelObjects* Cube;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALevelObjects>  CubeClass;

	UPROPERTY(BlueprintReadOnly)
	int Points = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void CreateFloorAndWalls();
	void CreateSnakeActor();
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void SpawnFood();
	UFUNCTION()
	int GetRandomCoordinate(float Side, float Offset);
	FVector FoodSpawnCoordinates(float Side, float Offset);

	UFUNCTION(BlueprintNativeEvent, Category = "ScoreSystem")
	void AddPoints();
	void AddPoints_Implementation();
};

