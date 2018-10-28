// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction_Interface.h"
#include "Interactable_Actor.generated.h"

UCLASS()
class CPP_PACKTTUT_API AInteractable_Actor : public AActor, public IInteraction_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable_Actor();

protected:
	// This is an override of the OnInteract method in the interface. The compiler knows to look for
	// the "implementation" postfix because it is a BlueprintNativeEvent in the header of the interface
	virtual void OnInteract_Implementation(AActor* Interactor);


	
};
