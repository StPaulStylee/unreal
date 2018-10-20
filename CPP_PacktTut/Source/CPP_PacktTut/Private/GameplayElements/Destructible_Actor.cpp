// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayElements/Destructible_Actor.h"
#include "Components/StaticMeshComponent.h"
#include "EngineMinimal.h"


// Sets default values
ADestructible_Actor::ADestructible_Actor()
{
	RootComponent = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	}
}

// Called when the game starts or when spawned
void ADestructible_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADestructible_Actor::OnDeath_Implementation()
{
}


