// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelObjects.generated.h"


class UStaticMeshComponent;
class UStaticMesh;


UCLASS()
class SNAKEGAME_API ALevelObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelObjects();


	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)

	UStaticMeshComponent* Cube;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UStaticMeshComponent> CubeClass;

	UPROPERTY()
	UMaterial* CubeMaterial;

	UPROPERTY()
	UStaticMesh* CubeMesh;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
