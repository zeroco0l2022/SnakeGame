// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelObjects.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ALevelObjects::ALevelObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	CubeMaterial = LoadObject<UMaterial>(nullptr, TEXT("Material'/Engine/MaterialTemplates/PerfTricks/VertexColorModulateTransition.VertexColorModulateTransition'"));

	FVector BlockLocator, BlockScale;
	FRotator BlockRotator;
	BlockScale.Set(20, 20, 0.1);
	BlockRotator.ZeroRotator;
	float X, Y, Z;
	X = 0;
	Y = 0;
	Z = 0;
	FString Name;

	

	for (int i = 0; i <= 4; i++)
	{
		Name = FString::FromInt(i);
		BlockLocator.Set(X, Y, Z);
		Cube = CreateDefaultSubobject<UStaticMeshComponent>(FName(Name));
		Cube->SetupAttachment(RootComponent);
		Cube->SetStaticMesh(CubeMesh);
		Cube->GetStaticMesh()->SetMaterial(0, CubeMaterial);
		Cube->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Cube->SetCollisionResponseToAllChannels(ECR_Overlap);
		Cube->SetRelativeLocation(BlockLocator);
		Cube->SetRelativeRotation(BlockRotator);

		if (i == 0)
		{
			SetRootComponent(Cube);
			Cube->SetRelativeScale3D(BlockScale);
			X = (Cube->GetStaticMesh()->GetBounds().BoxExtent.X);
			Y = 0;
			Z = X * BlockScale.X ;
			X *= -1;
			BlockRotator.Pitch += 90;
			continue;
		}
		else if (i % 2 != 0)
		{
			Y = X * (-1);
			X = 0;
			
		}
		else
		{
			X = Y;
			Y = 0;
			
		}
		BlockRotator.Roll += 90;
		BlockRotator.Pitch += 90;

	}

}


// Called when the game starts or when spawned
void ALevelObjects::BeginPlay()
{
	Super::BeginPlay(); 
}

// Called every frame
void ALevelObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



