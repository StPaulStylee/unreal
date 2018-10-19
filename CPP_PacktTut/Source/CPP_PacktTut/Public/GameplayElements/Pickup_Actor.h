// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup_Actor.generated.h"

UCLASS()
class CPP_PACKTTUT_API APickup_Actor : public AActor
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;
	
public:	
	// Sets default values for this actor's properties
	APickup_Actor();

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup_Actor")
		void OnCollection(class APlayer_Character* Collector);
	void OnCollection_Implementation(class APlayer_Character* Collector);

private:

	// This is a "Delegate" function. I don't know what that means because this resource is terrible. Anyways,
	// the UFUNCTION decorator must be present for the compiler to recognize a delegate method
	UFUNCTION()
		void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* CtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


};
