// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayElements/Interactable_Actor.h"


// Sets default values
AInteractable_Actor::AInteractable_Actor()
{

}

void AInteractable_Actor::OnInteract_Implementation(AActor * Interactor)
{
	// Debug message
	UE_LOG(LogTemp, Warning, TEXT("Interaction called from %s"), *Interactor->GetName());
}
