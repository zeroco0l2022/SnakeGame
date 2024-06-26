// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactible.h"
#include "Food.generated.h"



UCLASS()
class SNAKEGAME_API AFood : public AActor, public IInteractible
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();


	UStaticMeshComponent* Food;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UStaticMeshComponent> FoodClass;

	UPROPERTY()
	UMaterial* FoodMaterial;

	UPROPERTY()
	UStaticMesh* FoodMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AActor* Interactor, bool bIsHead) override;
};
