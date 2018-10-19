// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayElements/Pickup_Actor.h"
#include "Components/SphereComponent.h"
#include "Player_Character.h"

// Sets default values
APickup_Actor::APickup_Actor()
{
	// Create the collection box component and set it as the root component
	RootComponent = CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	if (CollectionSphere)
	{
		// Ignore every other channel
		CollectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
		// Only register pawns that overlap this actor and nothing else
		CollectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		// Bind the OnComonentBeginOverlap delage to our function
		CollectionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup_Actor::OnSphereOverlap);
	}

}

void APickup_Actor::OnCollection_Implementation(APlayer_Character * Collector)
{
	// Destory this actor
	Destroy();
}

void APickup_Actor::OnSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * CtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Check if the pawn that enter is our player pawn
	APlayer_Character* Player = Cast<APlayer_Character>(OtherActor);
	if (Player)
	{
		// Pass in the player refernce to OnCollected
		OnCollection(Player);
	}
}



